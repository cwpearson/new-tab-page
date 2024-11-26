cmake_minimum_required(VERSION 3.28)
Project(NewTabPage)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to MinSizeRel since none was specified")
  set(CMAKE_BUILD_TYPE MinSizeRel CACHE STRING "Choose the type of the build." FORCE)
endif()

add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
    COMMAND xxd --name html_data -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.html | sed 's/^unsigned/static const/' >  ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
    COMMAND xxd --name style_data -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.css | sed 's/^unsigned/static const/' >> ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
    COMMAND xxd --name script_data -i ${CMAKE_CURRENT_SOURCE_DIR}/static/index.js | sed 's/^unsigned/static const/' >> ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
    DEPENDS
        ${CMAKE_CURRENT_SOURCE_DIR}/static/index.html
        ${CMAKE_CURRENT_SOURCE_DIR}/static/index.css
        ${CMAKE_CURRENT_SOURCE_DIR}/static/index.js
)

add_custom_target(generate_sources
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp
)

add_executable(server server.cpp ${CMAKE_CURRENT_BINARY_DIR}/generated.hpp)
target_include_directories(server PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET server PROPERTY CXX_STANDARD 17) # string_view