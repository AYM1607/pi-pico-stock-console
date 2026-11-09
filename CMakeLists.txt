cmake_minimum_required(VERSION 3.13)

# Set the board before init-ing the sdk.
# Cache string to allow overriding through the command line.
# Options are in {PICO_SDK}/src/boards/include/boards/*.h
set(PICO_BOARD pimoroni_pico_plus2_w_rp2350 CACHE STRING "board type")
include(pico_sdk_import.cmake)

project(test_project C CXX ASM)


set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(test
    test.c
)

target_link_libraries(test
    pico_stdlib
    pico_cyw43_arch_none
    # Other options (when using wifi):
    # - pico_cyw43_arch_lwip_threadsafe_background
    # - pico_cyw43_arch_lwip_poll
    # - pico_cyw43_arch_lwip_sys_freertos
    # When using bluetooth:
    # - pico_btstack_cyw43
    # - pico_btstack_ble
)

pico_enable_stdio_usb(test 1)
pico_enable_stdio_uart(test 1)

pico_add_extra_outputs(test)
