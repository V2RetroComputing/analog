#include <pico/stdlib.h>
#include "hires_color_patterns.h"
#include "hires_dot_patterns.h"
#include "vgabuf.h"
#include "render.h"
#include "vgaout.h"

static void render_dhgr_line(bool p2, uint line);

#define _RGB333(r, g, b) ( \
    (((((uint)(r) * 256 / 36) + 128) / 256) << 6) | \
    (((((uint)(g) * 256 / 36) + 128) / 256) << 3) | \
    ((((uint)(b) * 256 / 36) + 128) / 256) \
)

uint16_t dhgr_palette[16] = {
    _RGB333(0x00,0x00,0x00),    // black    0000
    _RGB333(0x00,0x00,0xb4),    // d.blue   1000
    _RGB333(0x00,0x90,0x00),    // d.green  0100
    _RGB333(0x00,0x6c,0xd8),    // h.blue   1100
    _RGB333(0x6c,0x6c,0x00),    // brown    0010
    _RGB333(0x90,0x90,0x90),    // l.gray   1010
    _RGB333(0x00,0xd8,0x24),    // h.green  0110
    _RGB333(0x90,0xfc,0x90),    // aqua     1110
    _RGB333(0xd8,0x00,0x6c),    // magenta  0001
    _RGB333(0xb4,0x24,0xfc),    // h.violet 1001
    _RGB333(0x48,0x48,0x48),    // d.gray   0101
    _RGB333(0x6c,0x6c,0xff),    // l.blue   1101
    _RGB333(0xfc,0x6c,0x24),    // h.orange 0011
    _RGB333(0xff,0xd8,0xff),    // pink     1011
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

    bool p2 = !(soft_switches & SOFTSW_80STORE) && (soft_switches & SOFTSW_PAGE_2);

    for(uint line=0; line < 192; line++) {
        render_dhgr_line(p2, line);
    }

    render_border();
}


void __time_critical_func(render_mixed_dhgr)() {
    vga_prepare_frame();

    render_border();

    bool p2 = !(soft_switches & SOFTSW_80STORE) && (soft_switches & SOFTSW_PAGE_2);

    for(uint line=0; line < 160; line++) {
        render_dhgr_line(p2, line);
    }

    if(soft_switches & SOFTSW_80COL) {
        for(uint line=20; line < 24; line++) {
            render_text80_line(p2, line);
        }
    } else {
        for(uint line=20; line < 24; line++) {
            render_text40_line(p2, line);
        }
    }

    render_border();
}


static void __time_critical_func(render_dhgr_line)(bool p2, uint line) {
    struct vga_scanline *sl = vga_prepare_scanline();
    uint sl_pos = 0;
    uint i;

    const uint8_t *line_mema = (const uint8_t *)((p2 ? hgr_p2 : hgr_p1) + dhgr_line_to_mem_offset(line));
    const uint8_t *line_memb = (const uint8_t *)((p2 ? hgr_p4 : hgr_p3) + dhgr_line_to_mem_offset(line));

    // Pad 40 pixels on the left to center horizontally
    sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
    sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
    sl->data[sl_pos++] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word

    // DHGR is weird. Nuff said.
    uint32_t dots = 0;
    uint32_t pixeldata;
    int j;

    i = 0;
    while(i < 40) {
        // Load in the next 28 subpixels
        dots  = (line_memb[i] & 0x7f) << 0;
        dots |= (line_mema[i] & 0x7f) << 7;
        i++;
        dots |= (line_memb[i] & 0x7f) << 14;
        dots |= (line_mema[i] & 0x7f) << 21;
        i++;

        if(soft_switches & SOFTSW_MONOCHROME) {
            // Consume 6 pixels (24 subpixel bits)
            for(j = 0; j < 12; j++) {
                pixeldata = ((dots & 1) ? (0x1ff) : (0x000));
                dots >>= 1;
                pixeldata |= (((dots & 1) ? (0x1ff) : (0x000))) << 16;
                dots >>= 1;
                sl->data[sl_pos++] = pixeldata;
            }
        } else {
            // Consume 6 pixels (24 subpixel bits)
            for(j = 0; j < 3; j++) {
                pixeldata = (dhgr_palette[dots & 0xf] | THEN_EXTEND_3);
                dots >>= 4;
                pixeldata |= (dhgr_palette[dots & 0xf] | THEN_EXTEND_3) << 16;
                dots >>= 4;
                sl->data[sl_pos++] = pixeldata;
            }
        }

        // 4 subpixels roll over to the next block
        // Load in the next 28 subpixels
        dots |= (line_memb[i] & 0x7f) << 4;
        dots |= (line_mema[i] & 0x7f) << 11;
        i++;
        dots |= (line_memb[i] & 0x7f) << 18;
        dots |= (line_mema[i] & 0x7f) << 25;
        i++;

        if(soft_switches & SOFTSW_MONOCHROME) {
            // Consume 8 pixels (32 subpixel bits)
            for(j = 0; j < 16; j++) {
                pixeldata = ((dots & 1) ? (0x1ff) : (0x000));
                dots >>= 1;
                pixeldata |= (((dots & 1) ? (0x1ff) : (0x000))) << 16;
                dots >>= 1;
                sl->data[sl_pos++] = pixeldata;
            }
        } else {
            // Consume 8 pixels (32 subpixel bits)
            for(j = 0; j < 4; j++) {
                pixeldata = (dhgr_palette[dots & 0xf] | THEN_EXTEND_3);
                dots >>= 4;
                pixeldata |= (dhgr_palette[dots & 0xf] | THEN_EXTEND_3) << 16;
                dots >>= 4;
                sl->data[sl_pos++] = pixeldata;
            }
        }
    }

    // Pad 40 pixels on the left to center horizontally
    sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
    sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
    sl->data[sl_pos++] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word

    sl->length = sl_pos;
    sl->repeat_count = 1;
    vga_submit_scanline(sl);
}

