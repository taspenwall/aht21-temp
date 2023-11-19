cmake_minimum_required(VERSION 3.13)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)
project(read)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project



add_executable(read
        main.c
        )

# pull in common dependencies and additional i2c hardware support
target_link_libraries(read pico_stdlib hardware_i2c)

# create map/bin/hex file etc.
pico_add_extra_outputs(read)

# add url via pico_set_program_url
#example_auto_set_url(i2c_bus_scan)
