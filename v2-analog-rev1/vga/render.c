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

compat_t machinefont = MACHINE_II;
bool userfont = false;

uint16_t mono_colors[14] = {
    _RGB333(0x00, 0x00, 0x00), _RGB333(0xFF, 0xFF, 0xFF), // White Normal
    _RGB333(0xFF, 0xFF, 0xFF), _RGB333(0x00, 0x00, 0x00), // White Inverse
    _RGB333(0x00, 0x00, 0x00), _RGB333(0xFE, 0x7F, 0x00), // Amber Normal
    _RGB333(0xFE, 0x7F, 0x00), _RGB333(0x00, 0x00, 0x00), // Amber Inverse
    _RGB333(0x00, 0x00, 0x00), _RGB333(0x00, 0xBF, 0x00), // Green Normal
    _RGB333(0x00, 0xBF, 0x00), _RGB333(0x00, 0x00, 0x00), // Green Inverse
    _RGB333(0x35, 0x28, 0x79), _RGB333(0x6C, 0x5E, 0xB5), // Commodore
};

// Initialize the character generator ROM
void switch_font() {
    switch(current_machine) {
    default:
    case MACHINE_II:
        memcpy(character_rom, default_character_rom, 2048);
        break;
    case MACHINE_PRAVETZ:
        memcpy(character_rom, pravetz_character_rom, 2048);
        break;
    case MACHINE_IIE:
        memcpy(character_rom, appleiie_character_rom, 2048);
        break;
    case MACHINE_IIGS:
        memcpy(character_rom, appleiigs_character_rom, 2048);
        break;
    }
    machinefont = current_machine;
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

uint16_t status_timeout = 900;
uint8_t status_line[81];

void update_status_right(const char *str) {
    uint i, len;

    if(str != NULL) {
        len = strlen(str);
    } else {
        len = 0;
    }

    if(len < 80) {
        memset(status_line, ' ', 80 - len);
    } else {
        len = 80;
    }

    for(i = 0; i < len; i++) {
        status_line[(80-len) + i] = str[i];
    }

    status_timeout = 900;
}

void update_status_left(const char *str) {
    uint i, len;

    if(str != NULL) {
        len = strlen(str);
    } else {
        len = 0;
    }

    if(len < 80) {
        memset(status_line + len, ' ', 80 - len);
    } else {
        len = 80;
    }

    for(i = 0; i < len; i++) {
        status_line[i] = str[i];
    }

    status_timeout = 900;
}

void render_init() {
    int i;

    load_font();
    if(!userfont)
        switch_font();

    if((soft_switches & SOFTSW_MODE_MASK) == 0)
        internal_flags |= IFLAGS_TEST;
    terminal_tbcolor = 0xf0;
    terminal_border = 0x00;

    memcpy(terminal_character_rom, appleiie_character_rom, 2048);
    memset(status_line, 0, sizeof(status_line));

    render_test_init();
}

// Skip 48 lines to center vertically
void __time_critical_func(render_border)(uint count) {
    struct vga_scanline *sl = vga_prepare_scanline();
    uint sl_pos = 0;

    while(sl_pos < VGA_WIDTH/16) {
        sl->data[sl_pos] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 8 pixels per word
        sl_pos++;
    }

    sl->length = sl_pos;
    sl->repeat_count = count - 1;
    vga_submit_scanline(sl);
}

uint32_t screentimeout = 0;
uint32_t testdone = 0;

void __noinline __time_critical_func(render_loop)() {
    while(current_mode == MODE_VGACARD) {
        config_handler();

        if((busactive == 0) && (screentimeout > (15 * 60))) {
            vga_prepare_frame();
            render_border(480);
            memset(status_line, 0, sizeof(status_line));
            status_timeout = 0;
            vga_dpms_sleep();
            while(busactive == 0);
            vga_dpms_wake();
        } else {
            if(busactive == 0) {
                screentimeout++;
                if(screentimeout == 5) {
                    update_status_right("Going to sleep...");
                }
            } else {
                if(screentimeout >= 5) {
                    // Clear the sleep mode message
                    memset(status_line, 0, sizeof(status_line));
                    status_timeout = 0;
                }
                screentimeout = 0;
            }
            busactive = 0;

            if(!userfont && (machinefont != current_machine)) {
                switch_font();
            }

            update_text_flasher();

            if((soft_switches & SOFTSW_MONOCHROME) && (mono_palette != 0x7)) {
                text_fore = mono_colors[mono_palette*2+1];
                text_back = mono_colors[mono_palette*2];
                text_border = (mono_palette == 0x6) ? text_fore : text_back;
            } else {
                text_fore = lores_palette[TERMINAL_FORE];
                text_back = lores_palette[TERMINAL_BACK];
                text_border = lores_palette[TERMINAL_BORDER];
            }

            if(internal_flags & IFLAGS_TEST) {
                render_testpattern();
                // Automatically dismiss the test pattern when the Apple II is seen.
                if(((soft_switches & SOFTSW_MODE_MASK) != 0) && (testdone == 0)) {
                    internal_flags &= ~IFLAGS_TEST;
                    testdone = 1;
                    render_about_init();
                }
#if 0
            } else if(soft_switches & SOFTSW_TERMINAL) {
                render_terminal();
            } else if(soft_switches & SOFTSW_SHR) {
                render_shr();
#endif
            } else {
                vga_prepare_frame();

                render_border(24);
                if(status_line[0] != 0) {
                    render_status_line();
                    render_border(16);
                } else {
                    render_border(32);
                }

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

                render_border(48);
            }
        }
    }
}
