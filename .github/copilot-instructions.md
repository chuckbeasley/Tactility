# Copilot Instructions

## Build, test, and lint

- **Desktop/simulator build**: `cmake -S . -B build && cmake --build build`
- **Run all tests**: `ctest --test-dir build/Tests`
- **Build test targets only**: `cmake --build build --target build-tests`
- **Devicetree tests**: `python Buildscripts/DevicetreeCompiler/tests/test_integration.py` from `Buildscripts/DevicetreeCompiler/tests`
- **Single test**: run the matching binary from `build/Tests/<suite>/` or use `ctest --test-dir build/Tests -R <name>`

There is no repo-wide lint command defined at the top level. Formatting and style are governed by `.clang-format` and project-specific conventions in the relevant subproject.

## High-level architecture

- The repository is split between a shared core and platform-specific integration. `TactilityKernel` provides the HAL/kernel layer, `TactilityFreeRtos` adapts FreeRTOS usage, and `Tactility` contains the main app/runtime code.
- Top-level `CMakeLists.txt` switches between two worlds:
  - **ESP-IDF builds** when `ESP_IDF_VERSION` is set, pulling in `Firmware`, `Devices/<target>`, `Drivers`, `Modules`, `Platforms/platform-esp32`, and several bundled libraries as IDF components.
  - **Simulator/desktop builds** otherwise, using `Devices/simulator`, `Platforms/platform-posix`, SDL, FreeRTOS POSIX port, LVGL, and the desktop `Firmware`/`Tests` tree.
- Device behavior is described through devicetree YAML under `Devices/`, `Drivers/`, `Modules/`, `Platforms/`, and `TactilityKernel/bindings`. The build uses those definitions to assemble board-specific hardware configuration.
- Features are organized as modules and services. Notable service subtrees include `Tactility/Source/service/webserver`, `Tactility/Source/service/displayidle`, and app code such as `Tactility/Source/app/chat`.

## Key conventions

- Use the existing build split: `#if defined(ESP_IDF_VERSION)` / simulator code paths are intentional and should stay aligned.
- When adding ESP32-facing code, wire it through the relevant `Devices/<board>/devicetree.yaml` and matching driver/module binding instead of hardcoding board logic.
- Public service APIs live in `Tactility/.../Include`-style headers and are exercised by separate service tests under `Modules/*/tests`, `Tactility/Tests`, and `Tests/`.
- Screensaver and display idle behavior is centralized in `DisplayIdleService` and its settings; adding a new screensaver requires updating the enum, string conversions, UI options, and the service switch together.
- WebServer assets live in `/data/webserver/` with SD-card fallback at `/sdcard/tactility/webserver/`; the sync/version model is part of the service contract.
- Chat app packets are intentionally compact and versioned; protocol changes must preserve the wire header and payload parsing rules.
- Keep changes surgical. This repo already contains many third-party libraries under `Libraries/` and `managed_components/`; avoid editing them unless the task explicitly targets vendored code.
