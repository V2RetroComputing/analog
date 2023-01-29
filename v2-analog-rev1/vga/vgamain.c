#include <pico/stdlib.h>
#include <pico/multicore.h>
#include "common/abus.h"
#include "common/config.h"
#include "vga/businterface.h"
#include "vga/render.h"
#include "vga/vgaout.h"


void __noinline __time_critical_func(vgamain)() {
    vga_init();
    render_init();
    render_loop();
//    vga_deinit();
}
