#include <string.h>
#include <hardware/pio.h>
#include "common/config.h"
#include "common/buffers.h"
#include "abus.pio.h"
#include "z80/businterface.h"
#include "z80/z80buf.h"

volatile uint8_t *pcpi_reg = apple_memory + 0xC0C0;

static inline void __time_critical_func(pcpi_read)(uint32_t address) {
    switch(address & 0x7) {
    case 0: // Read Data from Z80
        clr_6502_stat;
        break;
    case 5: // Z80 Reset
        z80_res = 1;
        break;
    case 6: // Z80 INT
        //z80_irq = 1;
        break;
    case 7: // Z80 NMI
        z80_nmi = 1;
        break;
    }
}


static inline void __time_critical_func(pcpi_write)(uint32_t address, uint32_t value) {
    switch(address & 0x7) {
    case 0:
    case 2:
    case 3:
    case 4:
        break;
    case 1: // Write Data to Z80
        pcpi_reg[1] = value & 0xff;
        set_z80_stat;
        break;
    case 5:
    case 6:
    case 7:
        pcpi_read(address);
        break;
    }
}

void __time_critical_func(z80_businterface)(uint32_t address, uint32_t value) {
    // Reset the Z80 when the Apple II resets
    if(reset_state == 3) z80_res = 1;

    // Shadow parts of the Apple's memory by observing the bus write cycles
    if(CARD_SELECT) {
        if(CARD_DEVSEL) {
            pcpi_reg = apple_memory + (address & 0xFFF0);
            if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
                pcpi_write(address, value);
            } else {
                pcpi_read(address);
            }
        }
    }
}

