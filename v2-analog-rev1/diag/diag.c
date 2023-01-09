#include <string.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include "common/config.h"
#include "common/buffers.h"
#include "diag/businterface.h"

void diagmain() {
    uint16_t i;
    
    memset((uint8_t*)(apple_memory+0xC080), 0xC0, 0x10);
    memset((uint8_t*)(apple_memory+0xC090), 0xC1, 0x10);
    memset((uint8_t*)(apple_memory+0xC0A0), 0xC2, 0x10);
    memset((uint8_t*)(apple_memory+0xC0B0), 0xC3, 0x10);
    memset((uint8_t*)(apple_memory+0xC0C0), 0xC4, 0x10);
    memset((uint8_t*)(apple_memory+0xC0D0), 0xC5, 0x10);
    memset((uint8_t*)(apple_memory+0xC0E0), 0xC6, 0x10);
    memset((uint8_t*)(apple_memory+0xC0F0), 0xC7, 0x10);
    memset((uint8_t*)(apple_memory+0xC100), 0xC1, 0x100);
    memset((uint8_t*)(apple_memory+0xC200), 0xC2, 0x100);
    memset((uint8_t*)(apple_memory+0xC300), 0xC3, 0x100);
    memset((uint8_t*)(apple_memory+0xC400), 0xC4, 0x100);
    memset((uint8_t*)(apple_memory+0xC500), 0xC5, 0x100);
    memset((uint8_t*)(apple_memory+0xC600), 0xC6, 0x100);
    memset((uint8_t*)(apple_memory+0xC700), 0xC7, 0x100);
    memset((uint8_t*)(apple_memory+0xC800), 0xC8, 0x800);

    while(v2mode == MODE_DIAG) {
        sleep_ms(50);
    }
}

