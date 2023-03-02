#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include "common/config.h"
#include "serial/businterface.h"
#include "serial/serialbuffer.h"
#include "serial/serialrom.h"
#include "bsp/board.h"
#include "tusb.h"

void serialmain() {
    int c, i;

    // Copy SSC ROM
    memcpy((void*)(apple_memory + 0xC800), (void*)(ssc_rom), 0x800);
    memcpy((void*)(apple_memory + 0xC100), (void*)(ssc_rom + 0x700), 0x100);
    memcpy((void*)(apple_memory + 0xC200), (void*)(apple_memory + 0xC100), 0x600);

    switch(serialmux) {
    case SERIAL_USB:
        board_init();
        tusb_init();
        break;
    case SERIAL_WIFI:
        break;
    }

    while(current_mode == MODE_SERIAL) {
        config_handler();
        switch(serialmux) {
        case SERIAL_USB:
            tud_task();
            if(tud_cdc_n_available(0)) {
                uint8_t buf[64];

                uint32_t count = tud_cdc_n_read(0, buf, sizeof(buf));

                for(i = 0; i < count; i++)
                serial_rx_push(buf[i]);
            }
            break;
        case SERIAL_WIFI:
            break;
        case SERIAL_LOOP:
            if(!serial_tx_empty()) {
                c = serial_tx_pop();
                serial_rx_push(c);
            }
            break;
        }
    }
}
