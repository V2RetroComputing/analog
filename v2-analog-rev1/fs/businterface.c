#include <string.h>
#include <hardware/pio.h>
#include "common/config.h"
#include "common/buffers.h"
#include "abus.pio.h"
#include "fs/businterface.h"
#include "fs/fs.h"

volatile uint8_t fs_slot = 0;

void __time_critical_func(fs_businterface)(uint32_t address, uint32_t value) {
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
            }
        }

        if(CARD_IOSTROBE) {
            apple_memory[address] = value;
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
