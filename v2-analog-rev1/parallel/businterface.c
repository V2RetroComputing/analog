#include <string.h>
#include <hardware/pio.h>
#include "common/config.h"
#include "abus.pio.h"
#include "parallel/businterface.h"
#include "parallel/parallelbuffer.h"

volatile uint8_t parallel_tx_reg = 0x00;
volatile uint8_t parallel_strobe = 0x00;

static inline void __time_critical_func(parallel_write)(uint32_t address, uint32_t value) {
    uint8_t last_strobe = parallel_strobe;

    if(address & 0x1) {
        parallel_tx_reg = value;
    }

    if(address & 0x2) {
        parallel_strobe = 1;
    }

    if((last_strobe == 0) && (parallel_strobe == 1)) {
        parallel_tx_push(parallel_tx_reg);
    }

    if(address & 0x4) {
        parallel_strobe = 0;
    }
}

static inline void __time_critical_func(shadow_memory)(uint32_t address, uint32_t value) {
    // Shadow parts of the Apple's memory by observing the bus write cycles
    if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0) {
        if(CARD_SELECT) {
            if(CARD_DEVSEL) {
                parallel_write(address, value);
            } else if(address < 0xC000) {
                apple_memory[address] = value;
            }
            // Config memory in card slot-rom address space
            if(CARD_IOSEL) {
                config_memory[address & 0x1F] = value;
                if((address & 0xFF) == 0xFF)
                    config_handler();
            }
        }
    }
}


void __time_critical_func(parallel_businterface)() {
    while(v2mode == MODE_PARALLEL) {
        uint32_t value = pio_sm_get_blocking(CONFIG_ABUS_PIO, ABUS_MAIN_SM);
        uint32_t dout;
        uint32_t address = (value >> 10) & 0xffff;

        if((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) != 0) {
            if(CARD_SELECT) {
                if(CARD_DEVSEL) {
                    dout = ((address & 0xf) == 0) ? parallel_status : 0xff;
                } else {
                    dout = apple_memory[address];
                }

                // device read access
                pio_sm_put_blocking(CONFIG_ABUS_PIO, ABUS_DEVICE_READ_SM, dout);
            }
        }

        shadow_memory(address, value);
    }
}
