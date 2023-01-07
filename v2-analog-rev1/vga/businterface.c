#include <string.h>
#include <hardware/pio.h>
#include "common/config.h"
#include "abus.pio.h"
#include "vga/businterface.h"
#include "vga/vgabuf.h"


volatile uint8_t *videx_page = videx_memory;


static inline void __time_critical_func(videx_crtc_addr)(uint32_t value) {
}


static inline void __time_critical_func(videx_crtc_write)(uint32_t value) {
}


static inline void __time_critical_func(shadow_memory)(uint32_t address, uint32_t value) {
    // Shadow parts of the Apple's memory by observing the bus write cycles
    if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
        // Mirror Video Memory from MAIN & AUX banks
        if(terminal_switches && TERMINAL_AUX_WRITE) {
            if((address >= 0x400) && (address < 0xC00)) {
                private_memory[address] = value & 0xff;
            } else if((address >= 0x2000) && (address < 0x6000)) {
                private_memory[address] = value & 0xff;
            }
        } else {
            if((address >= 0x400) && (address < 0xC00)) {
                apple_memory[address] = value & 0xff;
            } else if((address >= 0x2000) && (address < 0x6000)) {
                apple_memory[address] = value & 0xff;
            }
        }

        // Videx 80 Column Card
        if(CARD_SELECT) {
            if((address >= 0xCC00) && (address < 0xD000)) {
                videx_page[(address & 0x3FF)] = value & 0xff;
            }

            if(CARD_DEVSEL) {
                switch(address & 0x01) {
                case 0x0:
                    videx_crtc_addr(value & 0xff);
                    break;
                case 0x1:
                    videx_crtc_write(value & 0xff);
                    break;
                }
            }

            // Config memory in card slot-rom address space
            if(CARD_IOSEL) {
                config_memory[address & 0x1F] = value;
                if((address & 0xFF) == 0xFF)
                    config_handler();
            }
        }
    }

    // Any access to Videx I/O sets the VRAM page visible in 0xCC00-0xD000
    if(CARD_DEVSEL) {
        videx_page = videx_memory + ((address & 0x0C) << 9);
    }

    // Shadow the soft-switches by observing all read & write bus cycles
    if((address & 0xff80) == 0xc000) {
        switch(address & 0x7f) {
        case 0x0c:
            if(terminal_switches & TERMINAL_IIE_REGS)
                terminal_switches &= ~((uint32_t)TERMINAL_80COL);
            break;
        case 0x0d:
            if(terminal_switches & TERMINAL_IIE_REGS)
                terminal_switches |= TERMINAL_80COL;
            break;
        case 0x22:
            if(terminal_switches & TERMINAL_IIGS_REGS)
                terminal_tbcolor = value & 0xff;
            break;
        case 0x34:
            if(terminal_switches & TERMINAL_IIGS_REGS)
                terminal_border = value & 0x0f;
            break;
        case 0x50:
            soft_switches &= ~((uint32_t)SOFTSW_TEXT_MODE);
            break;
        case 0x51:
            soft_switches |= SOFTSW_TEXT_MODE;
            break;
        case 0x52:
            soft_switches &= ~((uint32_t)SOFTSW_MIX_MODE);
            break;
        case 0x53:
            soft_switches |= SOFTSW_MIX_MODE;
            break;
        case 0x54:
            soft_switches &= ~((uint32_t)SOFTSW_PAGE_2);
            break;
        case 0x55:
            soft_switches |= SOFTSW_PAGE_2;
            break;
        case 0x56:
            soft_switches &= ~((uint32_t)SOFTSW_HIRES_MODE);
            break;
        case 0x57:
            soft_switches |= SOFTSW_HIRES_MODE;
            break;
        case 0x5e:
            if(terminal_switches & TERMINAL_IIE_REGS)
                terminal_switches |= TERMINAL_DGR;
            break;
        case 0x5f:
            if(terminal_switches & TERMINAL_IIE_REGS)
                terminal_switches &= ~((uint32_t)TERMINAL_DGR);
            break;
        }
    }
}


void __time_critical_func(vga_businterface)() {
    while(v2mode == MODE_VGACARD) {
        uint32_t value = pio_sm_get_blocking(CONFIG_ABUS_PIO, ABUS_MAIN_SM);
        uint32_t address = (value >> 10) & 0xffff;

        if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) != 0) {
            if(CARD_SELECT) {
                uint32_t dout;
                switch(address >> 10) {
                case (0xC000 >> 10):
                    dout = (address & 1) ? videx_crtc_reg : 0xff;
                    break;
                case (0xCC00 >> 10):
                    dout = videx_memory[address & 0x3FF];
                    break;
                default:
                    dout = apple_memory[address];
                }

                // device read access
                pio_sm_put_blocking(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM, dout);
            }
        }

        shadow_memory(address, value);
    }
}
