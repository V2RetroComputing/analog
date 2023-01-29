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

void __time_critical_func(serial_businterface)(uint32_t address, uint32_t value) {
    // Shadow parts of the Apple's memory by observing the bus write cycles
    if(CARD_SELECT) {
        if(CARD_DEVSEL) {
            if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
                serial_write(address, value);
            } else {
                serial_read(address);
            }
        }
    }
}
