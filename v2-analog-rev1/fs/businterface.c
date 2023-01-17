#include <string.h>
#include <hardware/pio.h>
#include "common/config.h"
#include "common/buffers.h"
#include "abus.pio.h"
#include "fs/businterface.h"
#include "fs/fs.h"

static inline void __time_critical_func(shadow_memory)(uint32_t address, uint32_t value) {
    // Shadow parts of the Apple's memory by observing the bus write cycles
    if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
        if(address < 0x200) {
            if((soft_switches & (SOFTSW_80STORE | SOFTSW_AUXZP)) == (SOFTSW_80STORE | SOFTSW_AUXZP)) {
                private_memory[address] = value & 0xff;
            } else {
                apple_memory[address] = value & 0xff;
            }
        } else if((address < 0xC000) || (address >= 0xD000)) {
            if((soft_switches & (SOFTSW_80STORE | SOFTSW_AUX_WRITE)) == (SOFTSW_80STORE | SOFTSW_AUX_WRITE)) {
                private_memory[address] = value & 0xff;
            } else {
                apple_memory[address] = value & 0xff;
            }
        }
    }

    if(CARD_SELECT) {
        if(CARD_DEVSEL) {
            if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
                apple_memory[address] = value;
                if((address & 0xCF8F) == 0xC08F)
                    fs_handler((address & 0x70) >> 4);
            }
        }

        if(CARD_IOSTROBE) {
            apple_memory[address] = value;
        }

        // Config memory in card slot-rom address space
        if(CARD_IOSEL) {
            if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
                config_memory[address & 0x1F] = value;
                if((address & 0xFF) == 0xFF)
                    config_handler();
            }
        }
    }

    // Shadow the soft-switches by observing all read & write bus cycles
    if((soft_switches & SOFTSW_IIE_REGS) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
        if((address & 0xff80) == 0xc000) {
            switch(address & 0x7f) {
            case 0x00:
                soft_switches &= ~SOFTSW_80STORE;
                break;
            case 0x01:
                soft_switches |= SOFTSW_80STORE;
                break;
            case 0x04:
                soft_switches &= ~SOFTSW_AUX_WRITE;
                break;
            case 0x05:
                soft_switches |= SOFTSW_AUX_WRITE;
                break;
            case 0x08:
                soft_switches &= ~SOFTSW_AUXZP;
                break;
            case 0x09:
                soft_switches |= SOFTSW_AUXZP;
                break;
            }
        }
    }
}


void __time_critical_func(fs_businterface)() {
    while(v2mode == MODE_DIAG) {
        uint32_t value = pio_sm_get_blocking(CONFIG_ABUS_PIO, ABUS_MAIN_SM);
        uint32_t dout;
        uint32_t address = (value >> 10) & 0xffff;

        if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) != 0) {
            if(CARD_SELECT) {
                dout = apple_memory[address];

                // device read access
                pio_sm_put_blocking(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM, dout);
            }
        }

        shadow_memory(address, value);
    }
}
