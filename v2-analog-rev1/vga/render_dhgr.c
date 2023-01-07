#include <pico/stdlib.h>
#include "hires_color_patterns.h"
#include "hires_dot_patterns.h"
#include "vgabuf.h"
#include "render.h"
#include "vgaout.h"

static void render_dhgr_line(uint line);

#define _RGB333(r, g, b) ( \
    (((((uint)(r) * 256 / 36) + 128) / 256) << 6) | \
    (((((uint)(g) * 256 / 36) + 128) / 256) << 3) | \
    ((((uint)(b) * 256 / 36) + 128) / 256) \
)

uint16_t dhgr_palette[16] = {
    _RGB333(0x00,0x00,0x00),    // black    0000
    _RGB333(0x00,0x00,0xb4),    // d.blue   1000
    _RGB333(0x00,0x48,0x00),    // d.green  0100
    _RGB333(0x00,0x90,0xfc),    // h.blue   1100
    _RGB333(0x24,0x24,0x00),    // brown    0010
    _RGB333(0x90,0x90,0x90),    // l.gray   1010
    _RGB333(0x00,0xd8,0x24),    // h.green  0110
    _RGB333(0x90,0xfc,0xb4),    // aqua     1110
    _RGB333(0x6c,0x00,0x6c),    // magenta  0001
    _RGB333(0xb4,0x24,0xfc),    // h.violet 1001
    _RGB333(0x48,0x48,0x48),    // d.gray   0101
    _RGB333(0x6c,0x6c,0xfc),    // l.blue   1101
    _RGB333(0xfc,0x48,0x00),    // h.orange 0011
    _RGB333(0xfc,0x6c,0xfc),    // pink     1011
    _RGB333(0xd8,0xd8,0x00),    // yellow   0111
    _RGB333(0xff,0xff,0xff),    // white    1111
};
#undef _RGB333

static uint dhgr_line_to_mem_offset(uint line) {
    return ((line & 0x07) << 10) | ((line & 0x38) << 4) | (((line & 0xc0) >> 6) * 40);
}


void __time_critical_func(render_dhgr)() {
    vga_prepare_frame();

    render_border();

    for(uint line=0; line < 192; line++) {
        render_dhgr_line(line);
    }

    render_border();
}


void __time_critical_func(render_mixed_dhgr)() {
    vga_prepare_frame();

    render_border();

    for(uint line=0; line < 160; line++) {
        render_dhgr_line(line);
    }

    if(terminal_switches & TERMINAL_80COL) {
        for(uint line=20; line < 24; line++) {
            render_terminal_line(line);
        }
    } else {
        for(uint line=20; line < 24; line++) {
            render_text_line(line);
        }
    }

    render_border();
}


static void __time_critical_func(render_dhgr_line)(uint line) {
    struct vga_scanline *sl = vga_prepare_scanline();
    uint sl_pos = 0;
    uint i;

    const uint8_t *page = (const uint8_t *)hires_memory;
    const uint8_t *line_mem = page + dhgr_line_to_mem_offset(line);

    // Pad 40 pixels on the left to center horizontally
    while(sl_pos < 40/8) {
        sl->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl_pos++;
    }

    // DHGR is weird. Nuff said.
    uint32_t dots = 0;
    uint32_t pixeldata;
    int j;

    i = 0;
    while(i < 40) {
        // Load in the next 28 subpixels
        dots  = (line_mem[i | 8192] & 0x7f) << 0;
        dots |= (line_mem[i] & 0x7f) << 7;
        i++;
        dots |= (line_mem[i | 8192] & 0x7f) << 14;
        dots |= (line_mem[i] & 0x7f) << 21;
        i++;

        // Consume 6 pixels (24 subpixel bits)
#if 0
        for(j = 0; j < 12; j++) {
            pixeldata = ((dots & 1) ? (text_fore) : (text_back));
            dots >>= 1;
            pixeldata |= (((dots & 1) ? (text_fore) : (text_back))) << 16;
            dots >>= 1;
            sl->data[sl_pos++] = pixeldata;
        }
#else
        for(j = 0; j < 3; j++) {
            pixeldata = (dhgr_palette[dots & 0xf] | THEN_EXTEND_3);
            dots >>= 4;
            pixeldata |= (dhgr_palette[dots & 0xf] | THEN_EXTEND_3) << 16;
            dots >>= 4;
            sl->data[sl_pos++] = pixeldata;
        }
#endif

        // 4 subpixels roll over to the next block
        // Load in the next 28 subpixels
        dots |= (line_mem[i | 8192] & 0x7f) << 4;
        dots |= (line_mem[i] & 0x7f) << 11;
        i++;
        dots |= (line_mem[i | 8192] & 0x7f) << 18;
        dots |= (line_mem[i] & 0x7f) << 25;
        i++;

        // Consume 8 pixels (32 subpixel bits)
#if 0
        for(j = 0; j < 16; j++) {
            pixeldata = ((dots & 1) ? (text_fore) : (text_back));
            dots >>= 1;
            pixeldata |= (((dots & 1) ? (text_fore) : (text_back))) << 16;
            dots >>= 1;
            sl->data[sl_pos++] = pixeldata;
        }
#else
        for(j = 0; j < 4; j++) {
            pixeldata = (dhgr_palette[dots & 0xf] | THEN_EXTEND_3);
            dots >>= 4;
            pixeldata |= (dhgr_palette[dots & 0xf] | THEN_EXTEND_3) << 16;
            dots >>= 4;
            sl->data[sl_pos++] = pixeldata;
        }
#endif
    }


    for(i = 0; i < 40/8; i++) {
        sl->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl_pos++;
    }

    sl->length = sl_pos;
    sl->repeat_count = 1;
    vga_submit_scanline(sl);
}
