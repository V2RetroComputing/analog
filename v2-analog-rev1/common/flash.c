#include <pico/stdlib.h>
#include <pico/multicore.h>
#include <hardware/flash.h>
#include <hardware/watchdog.h>
#include <hardware/resets.h>
#include "config.h"

#ifdef RASPBERRYPI_PICO_W
#include <pico/cyw43_arch.h>
#endif

void __time_critical_func(flash_reboot)() __attribute__ ((noreturn));

// Reboot the Pico
void __time_critical_func(flash_reboot)() {
    save_and_disable_interrupts();
    
    multicore_reset_core1();

    reset_block((1<<11) | (1<<10) | (1<<2));

    watchdog_enable(2, 1);
    for(;;);
}

