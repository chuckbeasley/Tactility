file(GLOB_RECURSE SOURCE_FILES source/*.c*)

idf_component_register(
    SRCS ${SOURCE_FILES}
    # esp_driver_i2c and freertos are for the board bring-up in source/module.cpp: the panel does not
    # respond until the AXP2101 rails are enabled and the CH32V003 expander has pulsed its reset pins.
    # axp2101-module is for the PMIC initialisation in source/board_pmic_init.cpp, which the boot-time
    # bring-up always runs. esp_partition is only needed by source/vbus_watch.cpp, whose flash stash
    # lives in the coredump partition; that file is compiled out unless
    # CONFIG_TT_WAVESHARE_C5_VBUS_WATCH is set (see Tactility/Kconfig), and the REQUIRE is kept because
    # esp_partition is part of the kernel's own partition handling anyway.
    REQUIRES TactilityKernel axp2101-module esp_driver_i2c esp_driver_gpio esp_partition esp_timer freertos
)
