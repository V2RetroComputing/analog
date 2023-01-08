#include <string.h>
#include <pico/stdlib.h>
#include <hardware/timer.h>
#include "common/config.h"
#include "vga/vgabuf.h"
#include "vga/render.h"
#include "vga/character_rom.h"
#include "vga/vgaout.h"

uint16_t text_fore;
uint16_t text_back;
uint16_t text_border;

void render_init() {
    int i;
    // Initialize the character generator ROM
    memcpy(character_rom, default_character_rom, sizeof(character_rom));
    soft_switches = 0;
    terminal_switches = TERMINAL_TEST | TERMINAL_IIE_REGS | TERMINAL_IIGS_REGS;
    terminal_tbcolor = 0x0f;
    terminal_border = 0x00;

    render_test_init();
}

uint32_t testdone=0;
void __noinline __time_critical_func(render_loop)() {
    while(v2mode == MODE_VGACARD) {
        update_text_flasher();

        text_fore = lores_palette[TERMINAL_FORE];
        text_back = lores_palette[TERMINAL_BACK];
        text_border = lores_palette[TERMINAL_BORDER];

        if(terminal_switches & TERMINAL_TEST) {
            render_testpattern();

            // Automatically dismiss the test pattern when the Apple II initializes
            // soft switches during startup.
            if(((soft_switches & SOFTSW_MODE_MASK) != 0) && (testdone == 0)) {
                terminal_switches &= (~(uint32_t)TERMINAL_TEST);
                testdone = 1;
                render_about_init();
            }
        } else {
            switch(soft_switches & SOFTSW_MODE_MASK) {
            case 0:
                if(terminal_switches & TERMINAL_DGR) {
                    render_dgr();
                } else {
                    render_lores();
                }
                break;
            case SOFTSW_MIX_MODE:
                if(terminal_switches & (TERMINAL_80COL | TERMINAL_DGR) == (TERMINAL_80COL | TERMINAL_DGR)) {
                    render_mixed_dgr();
                } else {
                    render_mixed_lores();
                }
                break;
            case SOFTSW_HIRES_MODE:
                if(terminal_switches & TERMINAL_DGR) {
                    render_dhgr();
                } else {
                    render_hires();
                }
                break;
            case SOFTSW_HIRES_MODE|SOFTSW_MIX_MODE:
                if(terminal_switches & TERMINAL_DGR) {
                    render_mixed_dhgr();
                } else {
                    render_mixed_hires();
                }
                break;
            default:
                if(terminal_switches & TERMINAL_80COL) {
                    render_terminal();
                } else {
                    render_text();
                }
                break;
            }
        }
    }
}
