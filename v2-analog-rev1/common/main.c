#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include "common/abus.h"
#include "common/config.h"
#include "common/modes.h"
#include "vga/businterface.h"
#include "z80/businterface.h"
#include "serial/businterface.h"
#include "parallel/businterface.h"
#include "pico_hal.h"

#ifdef RASPBERRYPI_PICO_W
#include <pico/cyw43_arch.h>
#endif

volatile uint8_t core1_running = 0;

static void core1_loop() {
    for(;;) {
        switch(v2mode) {
        case MODE_VGACARD:
            core1_running = 1;
            vga_businterface();
            core1_running = 0;
            break;
        case MODE_APPLICARD:
            core1_running = 1;
            z80_businterface();
            core1_running = 0;
            break;
        case MODE_SERIAL:
            core1_running = 1;
            serial_businterface();
            core1_running = 0;
            break;
        case MODE_PARALLEL:
            core1_running = 1;
            parallel_businterface();
            core1_running = 0;
            break;
        }
    }
}

static void core0_loop() {
    for(;;) {
        switch(v2mode) {
        case MODE_REBOOT:
            flash_reboot();
            break;
        case MODE_VGACARD:
            vgamain();
            break;
        case MODE_APPLICARD:
            z80main();
            break;
        case MODE_SERIAL:
            serialmain();
            break;
        case MODE_PARALLEL:
            parallelmain();
            break;
        default:
            v2mode = MODE_VGACARD;
        }

        while(core1_running) {
            sleep_ms(5);
        };
    }
}

int main() {
    // Adjust system clock for better dividing into other clocks
    set_sys_clock_khz(CONFIG_SYSCLOCK*1000, true);

    abus_init();
    
    // Sensible defaults if there is no config / fs
    default_config();

    // Try mounting the LittleFS, or format if it isn't there.
    if(pico_mount(0) == LFS_ERR_OK) {
        read_config();
    } else if(pico_mount(1) == LFS_ERR_OK) {
        read_config();
    }

    multicore_launch_core1(core1_loop);

    core0_loop();

    return 0;
}
