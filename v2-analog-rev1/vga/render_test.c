#include <string.h>
#include <pico/stdlib.h>
#include <hardware/timer.h>
#include "vga/vgabuf.h"
#include "vga/render.h"
#include "vga/vgaout.h"
#include "vga/logo.h"

#ifdef RASPBERRYPI_PICO_W
#include <pico/unique_id.h>
#endif

#define _PIXPAIR(p1, p2) ((uint32_t)(p1) | (((uint32_t)p2) << 16))

char error_message[32*24+1];

void render_test_init() {
    memset(error_message, ' ', 32*24);
    memcpy(error_message +   0, "HARDWARE:         V2 Analog Rev1", 32);
    memcpy(error_message +  32, "FIRMWARE:  1 Mar 2023 Build 0159", 32);

    memcpy(error_message + 128, "     Copyright (C) 2022-2023    ", 32);
    memcpy(error_message + 160, "           David Kuder          ", 32);

    memcpy(error_message + 256, "            based on            ", 32);
    memcpy(error_message + 288, "   AppleII-VGA by Mark Aikens   ", 32);

    memcpy(error_message + 416, "    no Apple II bus activity    ", 32);

    memcpy(error_message + 544, "     Turn off power & check     ", 32);
    memcpy(error_message + 576, "   for proper card insertion.   ", 32);

    memcpy(error_message + 704, "         serial number:         ", 32);

    // Get Pico's Flash Serial Number (Board ID) and terminating null.
    memcpy(error_message + 736, "         ", 8);
    pico_get_unique_board_id_string(error_message + 744, 17);
    memcpy(error_message + 760, "         ", 8);
}

// Clear the error message, in case the user sets 0x20 in terminal switches
// to show the about screen.
void render_about_init() {
    memset(error_message + 416, ' ', 32);
    memset(error_message + 544, ' ', 64);
}

static inline uint_fast8_t __time_critical_func(char_test_bits)(uint_fast8_t ch, uint_fast8_t glyph_line) {
    uint_fast8_t bits = terminal_character_rom[((uint_fast16_t)(ch & 0x7f) << 3) | glyph_line | 0x400];
    return bits & 0x7f;
}

void __noinline __time_critical_func(render_testpattern)() {
    vga_prepare_frame();

    for(uint line=0; line < VGA_HEIGHT;) {
        if(line == 19) {
            render_status_line();
            line += 16;
        } else {
            struct vga_scanline *sl = vga_prepare_scanline();
            uint sl_pos = 0;

            if((line == 0) || (line == VGA_HEIGHT-1) || (line == 36) || (line == VGA_HEIGHT-36-1)) {
                for(; sl_pos < VGA_WIDTH/8; sl_pos++) {
                    sl->data[sl_pos] = _PIXPAIR(0x1ff | THEN_EXTEND_3, 0x1ff | THEN_EXTEND_3);
                }
                sl->length = sl_pos;
            } else if(line == 1) {
                for(uint i=0; i < VGA_WIDTH/4; i++, sl_pos++) {
                    sl->data[sl_pos] = _PIXPAIR(0x1ff, 0);
                }
                for(uint i=0; i < VGA_WIDTH/4; i++, sl_pos++) {
                    sl->data[sl_pos] = _PIXPAIR(0, 0x1ff);
                }

                sl->length = sl_pos;
                sl->repeat_count = 16;
            } else if(line == VGA_HEIGHT-36) {
                for(uint i=0; i < VGA_WIDTH/4; i++, sl_pos++) {
                    sl->data[sl_pos] = _PIXPAIR(0x1ff, 0);
                }
                for(uint i=0; i < VGA_WIDTH/4; i++, sl_pos++) {
                    sl->data[sl_pos] = _PIXPAIR(0, 0x1ff);
                }

                sl->length = sl_pos;
                sl->repeat_count = 34;
            } else if((line >= 48) && (line < 48+128)) {
                sl->data[sl_pos++] = _PIXPAIR(0x1ff, 0 | THEN_EXTEND_6); // 8px
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_1, 0 | THEN_EXTEND_1); // 4px

                const uint z = (line - 48);

                if((z & 0xf) < 14) {
                    const uint g = z / 16;
                    for(uint b=0; b < 3; b++) {
                        for(uint r=0; r < 8; r++) {
                            const uint rgb = (r << 6) | (g << 3) | b;
                            sl->data[sl_pos++] = _PIXPAIR(0, rgb | THEN_EXTEND_6); // 8px
                            sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_6, 0); // 8px
                        }
                    }
                } else {
                    for(uint i=0; i < 24; i++) {
                        sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_7, 0 | THEN_EXTEND_7); // 400px
                    }
                }

                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_3, 0 | THEN_EXTEND_3); // 8px

                const uint w = (line - 48) << 1;
                for(uint i=0; i < 32; i+=2) {
                    uint32_t bits_a = char_test_bits(error_message[(w & 0x3E0) | i], (w>>2) & 0x7);
                    uint32_t bits_b = char_test_bits(error_message[(w & 0x3E0) | (i+1)], (w>>2) & 0x7);
                    uint32_t bits = (bits_a << 7) | bits_b;
                    for(uint j=0; j < 7; j++) {
                        uint32_t pixeldata = (bits & 0x2000) ? (0) : (0x1ff);
                        pixeldata |= (bits & 0x1000) ? ((0) << 16) : ((0x1ff) << 16);
                        bits <<= 2;
                        sl->data[sl_pos++] = pixeldata;
                    }
                }

                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_1, 0 | THEN_EXTEND_1); // 4px
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_6, 0x1ff); // 8px

                sl->repeat_count = 1;
                sl->length = sl_pos;
            } else if((line >= 176) && (line < 176+128)) {
                sl->data[sl_pos++] = _PIXPAIR(0x1ff, 0 | THEN_EXTEND_6); // 8px
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_1, 0 | THEN_EXTEND_1); // 4px

                const uint z = (line - 176);
                const uint g = z / 16;

                // center-left square
                if((z & 0xf) < 14) {
                    uint b = 3;
                    for(uint r=0; r < 8; r++) {
                        const uint rgb = (r << 6) | (g << 3) | b;
                        sl->data[sl_pos++] = _PIXPAIR(0, rgb | THEN_EXTEND_6);
                        sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_6, 0);
                    }
                } else {
                    for(uint i=0; i < 8; i++) {
                        sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_7, 0 | THEN_EXTEND_7);
                    }
                }

                // center square (logo)
                for(uint i=0; i < 32; i++) {
                    uint color1 = lores_palette[PicoPalLogo[((z & 0x7e) << 4) | i] >> 4];
                    uint color2 = lores_palette[PicoPalLogo[((z & 0x7e) << 4) | i] & 0xf];
                    sl->data[sl_pos++] = _PIXPAIR(color1 | THEN_EXTEND_1, color2 | THEN_EXTEND_1);
                }

                // center-right square
                if((z & 0xf) < 14) {
                    uint b = 4;
                    for(uint r=0; r < 8; r++) {
                        const uint rgb = (r << 6) | (g << 3) | b;
                        sl->data[sl_pos++] = _PIXPAIR(0, rgb | THEN_EXTEND_6);
                        sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_6, 0);
                    }
                } else {
                    for(uint i=0; i < 8; i++) {
                        sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_7, 0 | THEN_EXTEND_7);
                    }
                }

                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_3, 0 | THEN_EXTEND_3); // 8px

                const uint w = (line - 48) << 1;
                for(uint i=0; i < 32; i+=2) {
                    uint32_t bits_a = char_test_bits(error_message[(w & 0x3E0) | i], (w>>2) & 0x7);
                    uint32_t bits_b = char_test_bits(error_message[(w & 0x3E0) | (i+1)], (w>>2) & 0x7);
                    uint32_t bits = (bits_a << 7) | bits_b;
                    for(uint j=0; j < 7; j++) {
                        uint32_t pixeldata = (bits & 0x2000) ? (0) : (0x1ff);
                        pixeldata |= (bits & 0x1000) ? ((0) << 16) : ((0x1ff) << 16);
                        bits <<= 2;
                        sl->data[sl_pos++] = pixeldata;
                    }
                }

                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_1, 0 | THEN_EXTEND_1); // 4px
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_6, 0x1ff); // 8px

                sl->repeat_count = 1;
                sl->length = sl_pos;
            } else if((line >= 304) && (line < 304+128)) {
                sl->data[sl_pos++] = _PIXPAIR(0x1ff, 0 | THEN_EXTEND_6); // 8px
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_1, 0 | THEN_EXTEND_1); // 4px

                const uint z = (line - 304);

                if((z & 0xf) < 14) {
                    const uint g = z / 16;
                    for(uint b=5; b < 8; b++) {
                        for(uint r=0; r < 8; r++) {
                            const uint rgb = (r << 6) | (g << 3) | b;
                            sl->data[sl_pos++] = _PIXPAIR(0, rgb | THEN_EXTEND_6);
                            sl->data[sl_pos++] = _PIXPAIR(rgb | THEN_EXTEND_6, 0);
                        }
                    }
                } else {
                    for(uint i=0; i < 24; i++) {
                        sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_7, 0 | THEN_EXTEND_7); // 400px
                    }
                }

                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_3, 0 | THEN_EXTEND_3); // 8px

                const uint w = (line - 48) << 1;
                for(uint i=0; i < 32; i+=2) {
                    uint32_t bits_a = char_test_bits(error_message[(w & 0x3E0) | i], (w>>2) & 0x7);
                    uint32_t bits_b = char_test_bits(error_message[(w & 0x3E0) | (i+1)], (w>>2) & 0x7);
                    uint32_t bits = (bits_a << 7) | bits_b;
                    for(uint j=0; j < 7; j++) {
                        uint32_t pixeldata = (bits & 0x2000) ? (0) : (0x1ff);
                        pixeldata |= (bits & 0x1000) ? ((0) << 16) : ((0x1ff) << 16);
                        bits <<= 2;
                        sl->data[sl_pos++] = pixeldata;
                    }
                }

                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_1, 0 | THEN_EXTEND_1); // 4px
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_6, 0x1ff); // 8px

                sl->repeat_count = 1;
                sl->length = sl_pos;
            } else if(line < VGA_HEIGHT) {
                // black w/ white border
                sl->data[sl_pos++] = _PIXPAIR(0x1ff, 0 | THEN_EXTEND_6);
                for(uint i=0; i < 39; i++) {
                    sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_7, 0 | THEN_EXTEND_7);
                }
                sl->data[sl_pos++] = _PIXPAIR(0 | THEN_EXTEND_6, 0x1ff);

                sl->length = sl_pos;
            } else {
                sl->data[sl_pos++] = _PIXPAIR(0, 0);

                sl->length = sl_pos;
            }

            line += sl->repeat_count + 1;
            vga_submit_scanline(sl);
        }
    }
}

void __time_critical_func(render_status_line)() {
    for(uint glyph_line=0; glyph_line < 8; glyph_line++) {
        struct vga_scanline *sl = vga_prepare_scanline();
        uint8_t *line_buf = status_line;
        uint32_t bits;
        uint sl_pos = 0;

        // Pad 40 pixels on the left to center horizontally
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word

        for(uint col=0; col < 40; ) {
            // Grab 14 pixels from the next two characters
            if(*line_buf != 0) {
                bits = char_test_bits(*line_buf++, glyph_line) << 7;
            } else {
                bits = 0x7f << 7;
            }
            if(*line_buf != 0) {
                bits |= char_test_bits(*line_buf++, glyph_line);
            } else {
                bits |= 0x7f;
            }
            col++;

            // Translate each pair of bits into a pair of pixels
            for(int i=0; i < 7; i++) {
                uint32_t pixeldata = (bits & 0x2000) ? (0x000) : (0x1ff);
                pixeldata |= (bits & 0x1000) ?
                    ((uint32_t)0x000) << 16 :
                    ((uint32_t)0x1ff) << 16;
                bits <<= 2;

                sl->data[sl_pos] = pixeldata;
                sl_pos++;
            }
        }

        // Pad 40 pixels on the right to center horizontally
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word

        sl->length = sl_pos;
        sl->repeat_count = 1;
        vga_submit_scanline(sl);
    }
}
