file(GLOB_RECURSE SOURCE_FILES source/*.c*)

idf_component_register(
    SRCS ${SOURCE_FILES}
    # esp_driver_i2c and freertos are for the board bring-up in source/module.cpp: the panel does not
    # respond until the AXP2101 rails are enabled and the CH32V003 expander has pulsed its reset pins.
    # axp2101-module is for source/vbus_watch.cpp, which asks the PMIC driver whether VBUS is present
    # (its own API, not a guessed register bit) and re-applies the rail table when the cable is pulled.
    # esp_partition is its flash stash: the watch's log is written to the coredump partition because
    # its normal transport is the USB cable that the experiment unplugs.
    REQUIRES TactilityKernel axp2101-module esp_driver_i2c esp_driver_gpio esp_partition esp_timer freertos
)
