cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_ASM_COMPILE_OBJECT "${CMAKE_CXX_COMPILER_TARGET}")

include("Buildscripts/logo.cmake")

string(ASCII 27 Esc)
set(ColorReset "${Esc}[m")
set(Cyan "${Esc}[36m")

file(READ version.txt TACTILITY_VERSION)
add_compile_definitions(TT_VERSION="${TACTILITY_VERSION}")

# tactility_add_module() macro
include("Buildscripts/module.cmake")

# Determine device identifier and project location
include("Buildscripts/device.cmake")

if (DEFINED ENV{ESP_IDF_VERSION})
    init_tactility_globals("sdkconfig")
    get_property(TACTILITY_DEVICE_PROJECT GLOBAL PROPERTY TACTILITY_DEVICE_PROJECT)
    get_property(TACTILITY_DEVICE_ID GLOBAL PROPERTY TACTILITY_DEVICE_ID)
else ()
    set(TACTILITY_DEVICE_PROJECT "Devices/simulator")
    set(TACTILITY_DEVICE_ID "simulator")
    set_property(GLOBAL PROPERTY TACTILITY_DEVICE_PROJECT ${TACTILITY_DEVICE_PROJECT})
    set_property(GLOBAL PROPERTY TACTILITY_DEVICE_ID ${TACTILITY_DEVICE_ID})
endif ()

if (DEFINED ENV{ESP_IDF_VERSION})
    message("Using ESP-IDF ${Cyan}v$ENV{ESP_IDF_VERSION}${ColorReset}")
    include($ENV{IDF_PATH}/tools/cmake/project.cmake)

    set(COMPONENTS
        Tactility
        # Core dumps. Not a dependency of Tactility, so without naming it here the component is
        # excluded from the build entirely - which also excludes its Kconfig menu, making
        # CONFIG_ESP_COREDUMP_ENABLE_TO_FLASH not merely unset but non-existent and therefore
        # impossible to turn on. It brings its own config menu once it is in the build.
        espcoredump
    )
    set(EXTRA_COMPONENT_DIRS
        # Tactility must be discovered first: its CMakeLists.txt calls init_tactility_globals(),
        # which other components' CMakeLists.txt (e.g. lvgl-module) rely on having already run to
        # read back TACTILITY_DEVICE_ID/TACTILITY_DEVICE_PROJECT via get_property(). ESP-IDF's
        # requirements-discovery pass processes EXTRA_COMPONENT_DIRS in an isolated sub-process, in
        # the order listed here, so this must stay first now that Firmware no longer exists as a
        # separate component.
        "Tactility"
        "Devices/${TACTILITY_DEVICE_PROJECT}"
        "Drivers"
        "Modules"
        "Platforms/platform-esp32"
        "TactilityKernel"
        "TactilityKernelCpp"
        "TactilityFreeRtos"
        "Libraries/elf_loader"
        "Libraries/esp_epaper"
        "Libraries/lv_screenshot"
        "Libraries/minitar"
        "Libraries/minmea"
        "Libraries/QRCode"
    )

    set(EXCLUDE_COMPONENTS "Simulator")

    # panic_info_t is architecture-independent (esp_private/panic_internal.h) so this wrap applies
    # to every target; PanicHandler.cpp branches internally per architecture.
    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=esp_panic_handler" APPEND)

    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=read" APPEND)
    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=write" APPEND)
    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=close" APPEND)

    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=lv_button_create" APPEND)
    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=lv_dropdown_create" APPEND)
    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=lv_list_create" APPEND)
    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=lv_list_add_button" APPEND)
    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=lv_obj_create" APPEND)
    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=lv_obj_set_flex_flow" APPEND)
    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=lv_switch_create" APPEND)
    idf_build_set_property(LINK_OPTIONS "-Wl,--wrap=lv_textarea_create" APPEND)

else ()
    message("Building for sim target")
    # eps-idf generates these from Kconfig, but posix build isn't set up with Kconfig.
    add_compile_definitions(CONFIG_TT_DEVICE_ID="simulator")
    add_compile_definitions(CONFIG_TT_DEVICE_NAME="Simulator")
    add_compile_definitions(CONFIG_TT_DEVICE_VENDOR="")
    add_compile_definitions(CONFIG_TT_DEVICE_NAME_SIMPLE="Simulator")
    add_compile_definitions(CONFIG_TT_LAUNCHER_APP_ID="tactility.launcher")
    add_compile_definitions(CONFIG_TT_AUTO_START_APP_ID="")
    add_compile_definitions(CONFIG_TT_USER_DATA_LOCATION_INTERNAL)
endif ()

project(Tactility)

if (DEFINED ENV{ESP_IDF_VERSION})
    # PanicHandler.cpp's RISC-V callstack walker requires s0 to stay a frame pointer, which GCC
    # does not guarantee without this flag. CONFIG_ESP_SYSTEM_USE_FRAME_POINTER does not add it
    # (it only selects which ESP-IDF backtrace-printing function gets compiled), and can't be used
    # instead: the bootloader shares this project's sdkconfig with no per-subproject override, and
    # enabling it there overflows the bootloader's fixed partition budget. Setting the flag here via
    # idf_build_set_property() only affects this project's own configure, not the bootloader's
    # separate one, so it's naturally excluded. Gated to RISC-V since Xtensa never reads s0 this way.
    # Must run after project(Tactility) above - project() initializes default build specifications
    # that would otherwise overwrite this.
    if(CONFIG_IDF_TARGET_ARCH_RISCV)
        idf_build_set_property(COMPILE_OPTIONS "-fno-omit-frame-pointer" APPEND)
    endif()

    # ESP32-C5 deauth injection.
    # The stock IDF v6.1 libnet80211.a rejects deauth frames ("unsupport frame
    # type: 0c0"). patch_deauth_libnet.py applies the ESP32-C5 Deauther's
    # single-byte fix. Run it automatically before linking so a bare `idf.py
    # build` always produces a deauth-capable firmware. The script is idempotent
    # (a no-op once the byte is already 0x0c), so this is safe on every build.
    if(CONFIG_IDF_TARGET STREQUAL "esp32c5")
        set(TT_DEAUTH_PATCH_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/Tactility/Source/app/wifitoolbox/patch_deauth_libnet.py")
        if(EXISTS "${TT_DEAUTH_PATCH_SCRIPT}")
            find_package(Python3 COMPONENTS Interpreter QUIET)
            if(Python3_EXECUTABLE)
                add_custom_target(tactility_deauth_patch
                    COMMAND ${Python3_EXECUTABLE} "${TT_DEAUTH_PATCH_SCRIPT}"
                    VERBATIM
                    USES_TERMINAL
                    COMMENT "Patching ESP32-C5 libnet80211.a for deauth injection"
                )
                add_dependencies(Tactility.elf tactility_deauth_patch)
            endif()
        endif()
    endif()
endif ()

# Defined as regular project for PC and component for ESP
if (NOT DEFINED ENV{ESP_IDF_VERSION})
    add_subdirectory(Tactility)
    add_subdirectory(TactilityFreeRtos)
    add_subdirectory(TactilityKernel)
    add_subdirectory(TactilityKernelCpp)
    add_subdirectory(Platforms/platform-posix)
    add_subdirectory(Devices/simulator)
    add_subdirectory(Libraries/cJSON)
    add_subdirectory(Libraries/lv_screenshot)
    add_subdirectory(Libraries/QRCode)
    add_subdirectory(Libraries/minitar)
    add_subdirectory(Libraries/minmea)
    add_subdirectory(Modules/lvgl-module)
    add_subdirectory(Modules/c-symbols-module)
    add_subdirectory(Modules/cpp-symbols-module)
    add_subdirectory(Modules/crypt-module)
    add_subdirectory(Modules/freertos-module)
    add_subdirectory(Modules/gps-module)
    add_subdirectory(Modules/http-module)
    add_subdirectory(Modules/mbedtls-module)
    add_subdirectory(Modules/posix-symbols-module)
    add_subdirectory(Modules/pthread-module)
    add_subdirectory(Modules/service-module)
    add_subdirectory(Modules/app-module)
    add_subdirectory(Modules/app-posix-module)
    add_subdirectory(Modules/lvgl-window-manager-module)
    add_subdirectory(Drivers/gps-generic-module)
    add_subdirectory(Drivers/gps-meshtastic-module)

    # FreeRTOS
    set(FREERTOS_CONFIG_FILE_DIRECTORY ${PROJECT_SOURCE_DIR}/Devices/simulator/Source CACHE STRING "")
    set(FREERTOS_PORT GCC_POSIX CACHE STRING "")
    add_subdirectory(Libraries/FreeRTOS-Kernel)
    target_compile_definitions(freertos_kernel PUBLIC "projCOVERAGE_TEST=0")

    # EmbedTLS
    set(ENABLE_TESTING OFF)
    set(ENABLE_PROGRAMS OFF)
    add_subdirectory(Libraries/mbedtls)

    # SDL
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    add_subdirectory(Libraries/SDL) # Added as idf component for ESP and as library for other targets

    # LVGL
    set(CONFIG_LV_BUILD_DEMOS OFF CACHE BOOL "" FORCE)
    set(CONFIG_LV_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    add_subdirectory(Libraries/lvgl) # Added as idf component for ESP and as library for other targets
    target_link_libraries(lvgl PRIVATE SDL2-static)

    # Tests
    add_subdirectory(Tests)

endif ()
