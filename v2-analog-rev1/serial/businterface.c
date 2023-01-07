#include <string.h>
#include <hardware/pio.h>
#include "common/config.h"
#include "abus.pio.h"
#include "common/buffers.h"
#include "serial/businterface.h"
#include "serial/serialbuffer.h"

static inline void __time_critical_func(serial_read)(uint32_t address) {
    switch(address & 0x7) {
    case 8: // Read Data from RX FIFO
        serial_rx_advance();
        break;
    }
}

static inline void __time_critical_func(serial_write)(uint32_t address, uint32_t value) {
    switch(address & 0x7) {
    case 8: // Write Data to TX FIFO
        serial_tx_push(value & 0xff);
        break;
    case 9:
        serial_reg[9] &= ACIA_STATUS_RXOVER;
        break;
    }
}

static inline void __time_critical_func(shadow_memory)(uint32_t address, uint32_t value) {
    // Shadow parts of the Apple's memory by observing the bus write cycles
    if(CARD_SELECT) {
        if(CARD_DEVSEL) {
            if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
                serial_write(address, value);
            } else {
                serial_read(address);
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


void __time_critical_func(serial_businterface)() {
    while(v2mode == MODE_SERIAL) {
        uint32_t value = pio_sm_get_blocking(CONFIG_ABUS_PIO, ABUS_MAIN_SM);
        uint32_t dout;
        uint32_t address = (value >> 10) & 0xffff;

        if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) != 0) {
            if(CARD_SELECT) {
                if(CARD_DEVSEL) {
                    dout = serial_reg[address & 0xf];
                } else {
                    dout = apple_memory[address & 0xffff];
                }

                // device read access
                pio_sm_put_blocking(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM, dout);
            }
        }

        shadow_memory(address, value);
    }
}
