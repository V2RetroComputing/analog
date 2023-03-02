#include <pico/stdlib.h>
#include "vgabuf.h"
#include "render.h"
#include "vgaout.h"

uint16_t lores_dot_pattern[16] = {
    0x0000,
    0x2222,
    0x1111,
    0x3333,
    0x0888,
    0x2AAA,
    0x1999,
    0x3BBB,
    0x0444,
    0x2666,
    0x1555,
    0x3777,
    0x0CCC,
    0x2EEE,
    0x1DDD,
    0x3FFF,
};

uint16_t lores_palette[16] = {
    _RGB333(0x00,0x00,0x00),    // black
    _RGB333(0x6c,0x00,0x6c),    // magenta
    _RGB333(0x00,0x00,0xb4),    // d.blue
    _RGB333(0xb4,0x24,0xfc),    // h.violet
    _RGB333(0x00,0x48,0x00),    // d.green
    _RGB333(0x48,0x48,0x48),    // d.gray
    _RGB333(0x00,0x90,0xfc),    // h.blue
    _RGB333(0x6c,0x6c,0xfc),    // l.blue
    _RGB333(0x24,0x24,0x00),    // brown
    _RGB333(0xfc,0x48,0x00),    // h.orange
    _RGB333(0x90,0x90,0x90),    // l.gray
    _RGB333(0xfc,0x6c,0xfc),    // pink
    _RGB333(0x00,0xd8,0x24),    // h.green
    _RGB333(0xd8,0xd8,0x00),    // yellow
    _RGB333(0x90,0xfc,0xb4),    // aqua
    _RGB333(0xff,0xff,0xff),    // white
};


static void render_lores_line(bool p2, uint line);

//#define PAGE2SEL (!(soft_switches & SOFTSW_80STORE) && (soft_switches & SOFTSW_PAGE_2))
#define PAGE2SEL ((soft_switches & (SOFTSW_80STORE | SOFTSW_PAGE_2)) == SOFTSW_PAGE_2)

void __time_critical_func(render_lores)() {
    for(uint line=0; line < 24; line++) {
        render_lores_line(PAGE2SEL, line);
    }
}


void __time_critical_func(render_mixed_lores)() {
    for(uint line=0; line < 20; line++) {
        render_lores_line(PAGE2SEL, line);
    }

    for(uint line=20; line < 24; line++) {
        if(soft_switches & SOFTSW_80COL) {
            render_text80_line(PAGE2SEL, line);
        } else {
            render_text40_line(PAGE2SEL, line);
        }
    }
}


static void __time_critical_func(render_lores_line)(bool p2, uint line) {
    // Construct two scanlines for the two different colored cells at the same time
    struct vga_scanline *sl1 = vga_prepare_scanline();
    struct vga_scanline *sl2 = vga_prepare_scanline();
    uint sl_pos = 0;
    uint i, j;
    uint32_t color1, color2;

    const uint8_t *line_buf = (const uint8_t *)((p2 ? text_p2 : text_p1) + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40));

    // Pad 40 pixels on the left to center horizontally
    for(i = 0; i < 40/8; i++) {
        sl1->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl2->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl_pos++;
    }

    if(soft_switches & SOFTSW_MONOCHROME) {
        for(i = 0; i < 40; i+=2) {
            color1 = lores_dot_pattern[line_buf[i] & 0xf] << 14;
            color2 = lores_dot_pattern[(line_buf[i] >> 4) & 0xf] << 14;
            color1 |= lores_dot_pattern[line_buf[i+1] & 0xf];
            color2 |= lores_dot_pattern[(line_buf[i+1] >> 4) & 0xf];

            for(j = 0; j < 14; j++) {
                uint32_t pixeldata;

                pixeldata = (color1 & 0x8000000) ? (text_fore) : (text_back);
                pixeldata |= (color1 & 0x4000000) ? ((text_fore) << 16) : ((text_back) << 16);
                color1 <<= 2;
                sl1->data[sl_pos] = pixeldata;

                pixeldata = (color2 & 0x8000000) ? (text_fore) : (text_back);
                pixeldata |= (color2 & 0x4000000) ? ((text_fore) << 16) : ((text_back) << 16);
                sl2->data[sl_pos] = pixeldata;
                color2 <<= 2;

                sl_pos++;
            }
        }
    } else {
        for(i = 0; i < 40; i++) {
            color1 = lores_palette[line_buf[i] & 0xf];
            color2 = lores_palette[(line_buf[i] >> 4) & 0xf];

            // Each lores pixel is 7 hires pixels, or 14 VGA pixels wide
            sl1->data[sl_pos] = (color1|THEN_EXTEND_6) | ((color1|THEN_EXTEND_6) << 16);
            sl2->data[sl_pos] = (color2|THEN_EXTEND_6) | ((color2|THEN_EXTEND_6) << 16);
            sl_pos++;
        }
    }

    for(i = 0; i < 40/8; i++) {
        sl1->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl2->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl_pos++;
    }

    sl1->length = sl_pos;
    sl1->repeat_count = 7;
    vga_submit_scanline(sl1);

    sl2->length = sl_pos;
    sl2->repeat_count = 7;
    vga_submit_scanline(sl2);
}
