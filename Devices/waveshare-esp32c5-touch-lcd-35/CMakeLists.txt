file(GLOB_RECURSE SOURCE_FILES source/*.c*)

idf_component_register(
    SRCS ${SOURCE_FILES}
    # esp_driver_i2c and freertos are for the board bring-up in source/module.cpp: the panel does not
    # respond until the AXP2101 rails are enabled and the CH32V003 expander has pulsed its reset pins.
    REQUIRES TactilityKernel esp_driver_i2c freertos
)
