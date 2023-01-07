#include <string.h>
#include <hardware/pio.h>
#include "common/config.h"
#include "abus.pio.h"
#include "z80/businterface.h"
#include "z80/z80buf.h"


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


static inline void __time_critical_func(shadow_memory)(uint32_t address, uint32_t value) {
    // Shadow parts of the Apple's memory by observing the bus write cycles
    if(CARD_SELECT) {
        if(CARD_DEVSEL) {
            if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
                pcpi_write(address, value);
            } else {
                pcpi_read(address);
            }
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
}


void __time_critical_func(z80_businterface)() {
    while(v2mode == MODE_APPLICARD) {
        uint32_t value = pio_sm_get_blocking(CONFIG_ABUS_PIO, ABUS_MAIN_SM);
        uint32_t dout;
        uint32_t address = (value >> 10) & 0xffff;

        if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) != 0) {
            if(CARD_SELECT) {
                dout = pcpi_reg[address & 0x7];

                // device read access
                pio_sm_put_blocking(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM, dout);
            }
        }

        shadow_memory(address, value);
    }
}
