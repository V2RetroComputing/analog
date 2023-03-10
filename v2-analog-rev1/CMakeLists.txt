cmake_minimum_required(VERSION 3.16)

if(${CMAKE_CURRENT_BINARY_DIR} MATCHES "build-lc")
set(PICO_BOARD pico)
else()
set(PICO_BOARD pico_w)
endif()

set(PICO_TARGET_LINKER_SCRIPT memmap_ota.ld)

# Pull in SDK (must be before project)
include(cmake/pico_sdk_import.cmake)

project(v2-analog)
set(CMAKE_C_STANDARD 11)

pico_sdk_init()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DPICO_USE_MALLOC_MUTEX=1")

add_executable(v2-analog-${PICO_BOARD})

pico_generate_pio_header(v2-analog-${PICO_BOARD}
    ${CMAKE_CURRENT_SOURCE_DIR}/vga/vga.pio)
pico_generate_pio_header(v2-analog-${PICO_BOARD}
    ${CMAKE_CURRENT_SOURCE_DIR}/common/abus.pio)

target_sources(v2-analog-${PICO_BOARD} PUBLIC
    common/ota.o
    common/buffers.c
    common/flash.c
    common/abus.c
    common/config.c
    common/main.c
    common/usb_descriptors.c
    diag/businterface.c
    diag/diag.c
    fs/businterface.c
    fs/fs.c
    vga/vgamain.c
    vga/businterface.c
    vga/vgabuf.c
    vga/render.c
    vga/render_hires.c
    vga/render_lores.c
    vga/render_text.c
    vga/render_80col.c
    vga/render_dhgr.c
    vga/render_dgr.c
    vga/render_shr.c
    vga/render_test.c
    vga/terminal_rom.c
    vga/character_rom.c
    vga/vgaout.c
    z80/z80main.c
    z80/businterface.c
    z80/z80buf.c
    serial/businterface.c
    serial/serialbuffer.c
    serial/serial.c
    parallel/businterface.c
    parallel/parallelbuffer.c
    parallel/parallel.c
)

add_subdirectory(lib/littlefs)

target_include_directories(v2-analog-${PICO_BOARD} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(v2-analog-${PICO_BOARD} PUBLIC
    pico_multicore
    pico_stdlib
    littlefs-lib
    tinyusb_device
    tinyusb_board
    hardware_resets
    hardware_irq
    hardware_dma
    hardware_pio
)

if(${PICO_BOARD} MATCHES "pico_w")
target_link_libraries(v2-analog-${PICO_BOARD} PUBLIC
    pico_cyw43_arch_lwip_poll
)
endif(${PICO_BOARD} MATCHES "pico_w")

pico_enable_stdio_usb(v2-analog-${PICO_BOARD} 0)
pico_enable_stdio_uart(v2-analog-${PICO_BOARD} 0)

pico_add_extra_outputs(v2-analog-${PICO_BOARD})
