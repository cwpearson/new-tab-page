# new-tab-page

![](https://github.com/cwpearson/new-tab-page/blob/master/docs/example.png)

## Quick Start

Requires a C++17 compiler for `std::string_view`.

```bash
cmake -S . -B build
make -C build
```

## Running as a login service on MacOs

```bash
bash scripts/macos-install.sh
```

To uninstall, run

```bash
bash scripts/macos-uninstall.sh
```

## Use this as your New Tab page

### Firefox

1. Install [New Tab Override](https://addons.mozilla.org/en-US/firefox/addon/new-tab-override/) extension
2. Set "Option" to "custom URL"
3. Set "URL" to "http://localhost:8081"

## Roadmap

- [ ] kokkos/kokkos and cwpearson/kokkos
- [ ] change binary to `new-tab-page-server` so it has a better name in MacOS login items
