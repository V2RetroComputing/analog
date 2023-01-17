#include <string.h>
#include <pico/stdlib.h>
#include <hardware/timer.h>
#include "common/config.h"
#include "vga/vgabuf.h"
#include "vga/render.h"
#include "vga/character_rom.h"
#include "vga/vgaout.h"
#include "pico_hal.h"

uint16_t text_fore;
uint16_t text_back;
uint16_t text_border;

compat_t machinefont = APPLE_II;
bool userfont = false;

// Initialize the character generator ROM
void switch_font() {
    switch(machine) {
    default:
    case APPLE_II:
        memcpy(character_rom, default_character_rom, 2048);
        break;
    case APPLE_IIE:
        memcpy(character_rom, appleiie_character_rom, 2048);
        break;
    case APPLE_IIGS:
        memcpy(character_rom, appleiigs_character_rom, 2048);
        break;
    }
    machinefont = machine;
}

void load_font() {
    int file = pico_open("font", LFS_O_RDONLY);
    int br = 0;

    userfont = false;

    if(file < 0) {
        return;
    }

    br = pico_read(file, character_rom, 2048);
    if(br == 2048) {
        userfont = true;
    }

    pico_close(file);
    return;
}

void render_init() {
    int i;

    load_font();
    if(!userfont)
        switch_font();

    if((soft_switches & SOFTSW_MODE_MASK) == 0)
        soft_switches |= SOFTSW_TEST;
    terminal_tbcolor = 0xf0;
    terminal_border = 0x00;

    render_test_init();
}

uint32_t testdone=0;
void __noinline __time_critical_func(render_loop)() {
    while(v2mode == MODE_VGACARD) {
        if(!userfont && (machinefont != machine)) {
            switch_font();
        }

        update_text_flasher();

        text_fore = lores_palette[TERMINAL_FORE];
        text_back = lores_palette[TERMINAL_BACK];
        text_border = lores_palette[TERMINAL_BORDER];

        if(soft_switches & SOFTSW_TEST) {
            render_testpattern();

            // Automatically dismiss the test pattern when the Apple II initializes
            // soft switches during startup.
            if(((soft_switches & SOFTSW_MODE_MASK) != 0) && (testdone == 0)) {
                soft_switches &= ~SOFTSW_TEST;
                testdone = 1;
                render_about_init();
            }
        } else if(soft_switches & SOFTSW_VIDEX) {
            render_videx();
        } else if(soft_switches & SOFTSW_SHR) {
            render_shr();
        } else {
            switch(soft_switches & SOFTSW_MODE_MASK) {
            case 0:
                if(soft_switches & SOFTSW_DGR) {
                    render_dgr();
                } else {
                    render_lores();
                }
                break;
            case SOFTSW_MIX_MODE:
                if((soft_switches & (SOFTSW_80COL | SOFTSW_DGR)) == (SOFTSW_80COL | SOFTSW_DGR)) {
                    render_mixed_dgr();
                } else {
                    render_mixed_lores();
                }
                break;
            case SOFTSW_HIRES_MODE:
                if(soft_switches & SOFTSW_DGR) {
                    render_dhgr();
                } else {
                    render_hires();
                }
                break;
            case SOFTSW_HIRES_MODE|SOFTSW_MIX_MODE:
                if((soft_switches & (SOFTSW_80COL | SOFTSW_DGR)) == (SOFTSW_80COL | SOFTSW_DGR)) {
                    render_mixed_dhgr();
                } else {
                    render_mixed_hires();
                }
                break;
            default:
                render_text();
                break;
            }
        }
    }
}
