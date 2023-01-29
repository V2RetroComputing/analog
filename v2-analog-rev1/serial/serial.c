#include <stdio.h>
#include <string.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include "common/config.h"
#include "serial/businterface.h"
#include "serial/serialbuffer.h"

void serialmain() {
    int c;

    // Copy SSC ROM
    memcpy((void*)(apple_memory + 0xC800), (void*)(ssc_rom), 0x800);
    memcpy((void*)(apple_memory + 0xC100), (void*)(ssc_rom + 0x700), 0x100);
    memcpy((void*)(apple_memory + 0xC200), (void*)(apple_memory + 0xC100), 0x600);

    while(current_mode == MODE_SERIAL) {
        config_handler();
        switch(serialmux) {
#if 0        
        case SERIAL_USB:
            if(!stdio_usb_connected()) {
                sleep_ms(100);
            } else {
                c=getchar_timeout_us(0);
                if(c != PICO_ERROR_TIMEOUT) {
                    serial_rx_push(c);
                }
                if(!serial_tx_empty()) {
                    c = serial_tx_pop();
                    putchar(c);
                }
            }
            break;
#endif
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
