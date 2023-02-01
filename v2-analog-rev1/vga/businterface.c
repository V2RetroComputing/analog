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


void __time_critical_func(vga_businterface)(uint32_t address, uint32_t value) {
    // Shadow the soft-switches by observing all read & write bus cycles
    if((address & 0xff80) == 0xc000) {
        switch(address & 0x7f) {
        case 0x00:
            if((soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches &= ~SOFTSW_80STORE;
            }
            break;
        case 0x01:
            if((soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches |= SOFTSW_80STORE;
            }
            break;
        case 0x04:
            if((soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches &= ~SOFTSW_AUX_WRITE;
            }
            break;
        case 0x05:
            if((soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches |= SOFTSW_AUX_WRITE;
            }
            break;
        case 0x08:
            if((soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches &= ~SOFTSW_AUXZP;
            }
            break;
        case 0x09:
            if((soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches |= SOFTSW_AUXZP;
            }
            break;
        case 0x0c:
            if((soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches &= ~SOFTSW_80COL;
            }
            break;
        case 0x0d:
            if((soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches |= SOFTSW_80COL;
            }
            break;
        case 0x0e:
            if((soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches &= ~SOFTSW_ALTCHAR;
            }
            break;
        case 0x0f:
            if((soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches |= SOFTSW_ALTCHAR;
            }
            break;
        case 0x21:
            if((SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                if(value & 0x80) {
                    soft_switches |= SOFTSW_MONOCHROME;
                } else {
                    soft_switches &= ~SOFTSW_MONOCHROME;
                }
            }
            break;
        case 0x22:
            if((soft_switches & SOFTSW_IIGS_REGS) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                terminal_tbcolor = value & 0xff;
            }
            break;
        case 0x29:
            if((soft_switches & SOFTSW_IIGS_REGS) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches = (soft_switches & ~(SOFTSW_NEWVID_MASK << SOFTSW_NEWVID_SHIFT)) | ((value & SOFTSW_NEWVID_MASK) << SOFTSW_NEWVID_SHIFT);
            }
            break;
        case 0x34:
            if((soft_switches & SOFTSW_IIGS_REGS) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                terminal_border = value & 0x0f;
            }
            break;
        case 0x35:
            if((soft_switches & SOFTSW_IIGS_REGS) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches = (soft_switches & ~(SOFTSW_SHADOW_MASK << SOFTSW_SHADOW_SHIFT)) | ((value & SOFTSW_SHADOW_MASK) << SOFTSW_SHADOW_SHIFT);
            }
            break;
        case 0x50:
            soft_switches &= ~SOFTSW_TEXT_MODE;
            break;
        case 0x51:
            soft_switches |= SOFTSW_TEXT_MODE;
            break;
        case 0x52:
            soft_switches &= ~SOFTSW_MIX_MODE;
            break;
        case 0x53:
            soft_switches |= SOFTSW_MIX_MODE;
            break;
        case 0x54:
            soft_switches &= ~SOFTSW_PAGE_2;
            break;
        case 0x55:
            soft_switches |= SOFTSW_PAGE_2;
            break;
        case 0x56:
            soft_switches &= ~SOFTSW_HIRES_MODE;
            break;
        case 0x57:
            soft_switches |= SOFTSW_HIRES_MODE;
            break;
        case 0x5e:
            if(soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) {
                soft_switches |= SOFTSW_DGR;
            }
            break;
        case 0x5f:
            if(soft_switches & (SOFTSW_IIGS_REGS | SOFTSW_IIE_REGS)) {
                soft_switches &= ~SOFTSW_DGR;
            }
            break;
        case 0x7e:
            if((soft_switches & SOFTSW_IIE_REGS) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches |= SOFTSW_IOUDIS;
            }
            break;
        case 0x7f:
            if((soft_switches & SOFTSW_IIE_REGS) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
                soft_switches &= ~SOFTSW_IOUDIS;
            }
            break;
        }
        return;
    }

    // Shadow parts of the Apple's memory by observing the bus write cycles
    if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
        // Mirror Video Memory from MAIN & AUX banks
        if(soft_switches & SOFTSW_LINEARIZE) {
            if((address >= 0x2000) && (address < 0xC000)) {
                private_memory[address] = value & 0xff;
                return;
            }
        }
        
        if(soft_switches & SOFTSW_80STORE) {
            if(soft_switches & SOFTSW_PAGE_2) {
                if((address >= 0x400) && (address < 0x800)) {
                    private_memory[address] = value & 0xff;
                    return;
                } else if((soft_switches & SOFTSW_HIRES_MODE) && (address >= 0x2000) && (address < 0x4000)) {
                    private_memory[address] = value & 0xff;
                    return;
                }
            }
        } else if(soft_switches & SOFTSW_AUX_WRITE) {
            if((address >= 0x200) && (address < 0xC000)) {
                private_memory[address] = value & 0xff;
                return;
            }
        }

        if((address >= 0x200) && (address < 0xC000)) {
            apple_memory[address] = value & 0xff;
            return;
        }

#if 0
        // Videx 80 Column Card
        if(CARD_SELECT) {
            if((address >= 0xC800) && (address < 0xCC00)) {
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
        }
#endif
    }

#if 0
    // Any access to Videx I/O sets the VRAM page visible in 0xCC00-0xD000
    if(CARD_SELECT && CARD_DEVSEL) {
        videx_page = videx_memory + ((address & 0x0C) << 9);
    }
#endif
}
