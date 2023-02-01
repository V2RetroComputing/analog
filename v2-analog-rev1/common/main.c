#include <stdio.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <hardware/pio.h>
#include "common/abus.h"
#include "common/config.h"
#include "common/modes.h"
#include "common/buffers.h"
#include "common/flash.h"
#include "pico_hal.h"

#ifdef RASPBERRYPI_PICO_W
#include <pico/cyw43_arch.h>
#endif

static void __time_critical_func(core1_loop)() {
    for(;;) {
        uint32_t value = pio_sm_get_blocking(CONFIG_ABUS_PIO, ABUS_MAIN_SM);
        uint32_t address = (value >> 10) & 0xffff;

        // device read access
        if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) != 0) {
            if(CARD_SELECT) {
                pio_sm_put_blocking(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM, apple_memory[address]);
            }
        }
        
        busactive = 1;

        if(CARD_SELECT) {
            if(CARD_DEVSEL) {
                cardslot = (address >> 4) & 0x7;
            } else if(CARD_IOSEL) {
                cardslot = (address >> 8) & 0x7;

                // Config memory in card slot-rom address space
                if(((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) && ((address & 0xF0) == 0xF0)) {
                    apple_memory[address] = value;
                }
            }
        }

        switch(current_mode) {
        case MODE_DIAG:
            diag_businterface(address, value);
            break;
        case MODE_FS:
            fs_businterface(address, value);
            break;
        case MODE_VGACARD:
            vga_businterface(address, value);
            break;
        case MODE_APPLICARD:
            z80_businterface(address, value);
            break;
        case MODE_SERIAL:
            serial_businterface(address, value);
            break;
        case MODE_PARALLEL:
            parallel_businterface(address, value);
            break;
        }

        if(current_machine == MACHINE_AUTO) {
            if((apple_memory[0x0417] == 0xE7) && (apple_memory[0x416] == 0xC9)) { // Apple IIgs
                current_machine = MACHINE_IIGS;
                soft_switches &= ~SOFTSW_IIE_REGS;
                soft_switches |= SOFTSW_IIGS_REGS;
            } else if((apple_memory[0x0417] == 0xE5) && (apple_memory[0x416] == 0xAF)) { // Apple //e Enhanced
                current_machine = MACHINE_IIE;
                soft_switches |= SOFTSW_IIE_REGS;
                soft_switches &= ~SOFTSW_IIGS_REGS;
            } else if((apple_memory[0x0415] == 0xDD) && (apple_memory[0x413] == 0xE5)) { // Apple //e Unenhanced
                current_machine = MACHINE_IIE;
                soft_switches |= SOFTSW_IIE_REGS;
                soft_switches &= ~SOFTSW_IIGS_REGS;
            } else if(apple_memory[0x0410] == 0xD0) { // Apple II/Plus/J-Plus with Autostart
                current_machine = MACHINE_II;
                soft_switches &= ~(SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS);
            } else if((apple_memory[0x07D0] == 0xAA) && (apple_memory[0x07D1] == 0x60)) { // Apple II without Autostart
                current_machine = MACHINE_II;
                soft_switches &= ~(SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS);
            } else if(apple_memory[0x0410] == 0xF2) { // Pravetz!
                current_machine = MACHINE_PRAVETZ;
                soft_switches &= ~(SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS);
            }
        }
    }
}

static void core0_loop() {
    for(;;) {
        switch(current_mode) {
        case MODE_DIAG:
            diagmain();
            break;
        case MODE_FS:
            fsmain();
            break;
        default:
            current_mode = MODE_VGACARD;
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
        }
    }
}

int main() {
    // Adjust system clock for better dividing into other clocks
    set_sys_clock_khz(CONFIG_SYSCLOCK*1000, true);

    abus_init();

    multicore_launch_core1(core1_loop);

    memcpy((uint8_t*)apple_memory+0xC1F0, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFFV2ANALOG", 16);
    memcpy((uint8_t*)apple_memory+0xC2F0, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFFV2ANALOG", 16);
    memcpy((uint8_t*)apple_memory+0xC3F0, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFFV2ANALOG", 16);
    memcpy((uint8_t*)apple_memory+0xC4F0, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFFV2ANALOG", 16);
    memcpy((uint8_t*)apple_memory+0xC5F0, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFFV2ANALOG", 16);
    memcpy((uint8_t*)apple_memory+0xC6F0, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFFV2ANALOG", 16);
    memcpy((uint8_t*)apple_memory+0xC7F0, "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFFV2ANALOG", 16);
    
    // Sensible defaults if there is no config / fs
    default_config();

    // Try mounting the LittleFS, or format if it isn't there.
    if(pico_mount(0) == LFS_ERR_OK) {
        read_config();
    } else {
        pico_mount(1);
    }

    current_mode = cfg_mode;

    core0_loop();

    return 0;
}
