cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(lcd-lib C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

add_executable(blink
		main.c
		led8seg4.c
	)

# pull in common dependencies
target_link_libraries(blink
		pico_stdlib
		hardware_adc
		hardware_pwm
	)
# create map/bin/hex file etc.
pico_add_extra_outputs(blink)

