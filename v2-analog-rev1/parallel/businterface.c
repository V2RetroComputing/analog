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

void __time_critical_func(parallel_businterface)(uint32_t address, uint32_t value) {
    // Shadow parts of the Apple's memory by observing the bus write cycles
    if((CARD_SELECT) && (CARD_DEVSEL) && ((value & (1u << CONFIG_PIN_APPLEBUS_RW-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)) {
        parallel_write(address, value);
    }
}
