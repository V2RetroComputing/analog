#include <string.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include "common/config.h"
#include "parallel/businterface.h"
#include "parallel/parallelbuffer.h"
#include "parallel/grappler_rom.h"


void parallelmain() {
    // Copy Grappler ROM to Slot ROM areas
    memcpy((void*)(apple_memory + 0xC100), (void*)(grappler_rom + 0x100), 0x700);

    // Copy Grappler ROM to Extended ROM area
    memcpy((void*)(apple_memory + 0xC800), (void*)(grappler_rom), 0x800);

    while(current_mode == MODE_PARALLEL) {
        config_handler();
        sleep_ms(50);
    }
}
