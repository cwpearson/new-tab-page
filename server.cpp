#include <functional>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <unordered_map>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "generated.hpp"

class Request {
public:
    std::string method;
    std::string path;
    std::string version;
    std::unordered_map<std::string, std::string> headers;
    std::string body;

    static Request parse(const char* buffer, size_t length) {
        Request request;
        std::istringstream stream(std::string(buffer, length));
        std::string line;

        // Parse request line
        if (std::getline(stream, line)) {
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            std::istringstream requestLine(line);
            requestLine >> request.method >> request.path >> request.version;
        }

        // Parse headers
        while (std::getline(stream, line) && line != "\r" && line != "") {
            line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());
            auto colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string key = line.substr(0, colonPos);
                std::string value = line.substr(colonPos + 1);
                
                // Trim leading/trailing whitespace
                value.erase(0, value.find_first_not_of(" "));
                value.erase(value.find_last_not_of(" ") + 1);
                
                request.headers[key] = value;
            }
        }

        // Parse body if Content-Length is present
        auto contentLength = request.headers.find("Content-Length");
        if (contentLength != request.headers.end()) {
            size_t length = std::stoul(contentLength->second);
            std::string body;
            char c;
            while (stream.get(c) && body.length() < length) {
                body += c;
            }
            request.body = body;
        }

        return request;
    }

    void debug_print() const {
        std::cout << "Method: " << method << "\n";
        std::cout << "Path: " << path << "\n";
        std::cout << "Version: " << version << "\n";
        std::cout << "Headers:\n";
        for (const auto& header : headers) {
            std::cout << "  " << header.first << ": " << header.second << "\n";
        }
        std::cout << "Body: " << body << "\n";
    }
};

class Response {
public:
    Response(int code, std::unordered_map<std::string, std::string> headers, const std::string_view body) : code_(code), headers_(headers), body_(body) {}

    std::string to_string() {

        std::stringstream ss;
        ss << "HTTP/1.1 " << code_ << "  OK\r\n";
        for (const auto &kv : headers_) {
            ss << kv.first << ": " << kv.second << "\r\n";
        }
        ss << "\r\n";
        ss << body_;
        return ss.str();
    }

    int code() const {return code_;}

private:
    int code_;
    std::unordered_map<std::string, std::string> headers_;
    std::string body_;
};

class Context {
public:
    Context(const Request &req) : req_(req) {}

    Response text_html(const std::string_view s) const {
        return Response(
            200,
            { {"Content-Type", "text/html"}, {"Connection", "close"}},
            s
        );
    }

    Response text_css(const std::string_view s) const {
        return Response(
            200,
            { {"Content-Type", "text/css"}, {"Connection", "close"}},
            s
        );
    }

    Response application_javascript(const std::string_view s) const {
        return Response(
            200,
            { {"Content-Type", "application/javascript"}, {"Connection", "close"}},
            s
        );
    }

private:
    Request req_;

};

class HTTPServer {
private:
    int server_fd;
    int port;
    struct sockaddr_in address;
    static constexpr int BUFFER_SIZE = 1024;

    using handler_type = std::function<Response(const Context &)>;

    std::unordered_map<std::string, handler_type> routes_;

public:
    HTTPServer(int port = 8080) : port(port) {
        // Create socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
            throw std::runtime_error("Socket creation failed");
        }

        // Set socket options
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            throw std::runtime_error("Set socket option failed");
        }

        // Setup server address structure
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Bind socket to port
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            throw std::runtime_error("Bind failed");
        }
    }

    void get(const std::string &path, handler_type &&handler) {
        routes_[path] = std::move(handler);
    }

    void start() {
        // Listen for incoming connections
        if (listen(server_fd, 3) < 0) {
            throw std::runtime_error("Listen failed");
        }

        std::cout << "Server listening on port " << port << std::endl;

        while (true) {
            handleConnection();
        }
    }

    ~HTTPServer() {
        close(server_fd);
    }

private:
    void handleConnection() {
        int new_socket;
        int addrlen = sizeof(address);
        char buffer[BUFFER_SIZE] = {0};

        // Accept incoming connection
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            std::cerr << "Accept failed" << std::endl;
            return;
        }

        // Read incoming request
        read(new_socket, buffer, BUFFER_SIZE);
        // std::cout << "Received request:\n" << buffer << std::endl;

        auto req = Request::parse(buffer, BUFFER_SIZE);
        // req.debug_print();

        Context ctx(req);

        Response response(404, {{"Content-Type", "text"}}, "not found");

        if (auto it = routes_.find(req.path); it != routes_.end()) {
            response = it->second(ctx);
        }

        std::cout << req.method << " " << req.path << " " << response.code() << "\n";

        std::string responseStr = response.to_string();
        send(new_socket, responseStr.c_str(), responseStr.length(), 0);
        close(new_socket);
    }
};


Response root_handler(const Context &c) {
    return c.text_html(std::string_view(html_data, html_data_len));
}
Response style_handler(const Context &c) {
    return c.text_css(std::string_view(style_data, style_data_len));
}
Response script_handler(const Context &c) {
    return c.application_javascript(std::string_view(script_data, script_data_len));
}

int main(int argc, char **argv) {

    int port = 8080;
    if (argc >= 2) {
        port = std::atoi(argv[1]);
    }

    try {
        HTTPServer server(port);
        server.get("/", root_handler);
        server.get("/index.css", style_handler);
        server.get("/index.js", script_handler);

        server.start();
    } catch (const std::exception& e) {
        std::cerr << "Server error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}