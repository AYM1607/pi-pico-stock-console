cmake_minimum_required(VERSION 3.13)

# Set the board before init-ing the sdk.
# Cache string to allow overriding through the command line.
# Options are in {PICO_SDK}/src/boards/include/boards/*.h
set(PICO_BOARD pimoroni_pico_plus2_w_rp2350 CACHE STRING "board type")
include(pico_sdk_import.cmake)

project(stock_console C CXX ASM)


set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)
pico_sdk_init()

add_executable(stock_console
    src/main.c
)

target_compile_definitions(stock_console PRIVATE
    WIFI_SSID="$ENV{WIFI_SSID}"
    WIFI_PASSWORD="$ENV{WIFI_PASSWORD}"
    ALPHA_VANTAGE_API_KEY="$ENV{ALPHA_VANTAGE_API_KEY}"
)

target_link_libraries(stock_console
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    # Other options (when using wifi):
    # - pico_cyw43_arch_lwip_poll
    # - pico_cyw43_arch_lwip_sys_freertos
    # - pico_cyw43_arch_none
    # When using bluetooth:
    # - pico_btstack_cyw43
    # - pico_btstack_ble
    pico_lwip_http
    pico_lwip_mbedtls
    pico_mbedtls
)

target_include_directories(stock_console PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include
)

pico_enable_stdio_usb(stock_console 1)
pico_enable_stdio_uart(stock_console 0)

pico_add_extra_outputs(stock_console)
