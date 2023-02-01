#include <pico/stdlib.h>
#include "vgabuf.h"
#include "render.h"
#include "vgaout.h"

//#define PAGE2SEL (!(soft_switches & SOFTSW_80STORE) && (soft_switches & SOFTSW_PAGE_2))
#define PAGE2SEL ((soft_switches & (SOFTSW_80STORE | SOFTSW_PAGE_2)) == SOFTSW_PAGE_2)

extern uint16_t lores_palette[16];

static void render_dgr_line(bool p2, uint line);

void __time_critical_func(render_dgr)() {
    for(uint line=0; line < 24; line++) {
        render_dgr_line(PAGE2SEL, line);
    }
}


void __time_critical_func(render_mixed_dgr)() {
    for(uint line=0; line < 20; line++) {
        render_dgr_line(PAGE2SEL, line);
    }

    for(uint line=20; line < 24; line++) {
        if(soft_switches & SOFTSW_80COL) {
            render_text80_line(PAGE2SEL, line);
        } else {
            render_text40_line(PAGE2SEL, line);
        }
    }
}


static void __time_critical_func(render_dgr_line)(bool p2, uint line) {
    // Construct two scanlines for the two different colored cells at the same time
    struct vga_scanline *sl1 = vga_prepare_scanline();
    struct vga_scanline *sl2 = vga_prepare_scanline();
    uint sl_pos = 0;
    uint i;

    const uint8_t *line_bufa = (const uint8_t *)((p2 ? text_p2 : text_p1) + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40));
    const uint8_t *line_bufb = (const uint8_t *)((p2 ? text_p4 : text_p3) + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40));

    // Pad 40 pixels on the left to center horizontally
    for(i = 0; i < 40/8; i++) {
        sl1->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl2->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl_pos++;
    }

    for(i=0; i < 40; i++) {
        uint32_t color1 = lores_palette[line_bufb[i] & 0xf];
        uint32_t color2 = lores_palette[(line_bufb[i] >> 4) & 0xf];
        uint32_t color3 = lores_palette[line_bufa[i] & 0xf];
        uint32_t color4 = lores_palette[(line_bufa[i] >> 4) & 0xf];

        // Each double lores pixel is 7 double hires pixels, or 7 VGA pixels wide
        sl1->data[sl_pos] = (color1|THEN_EXTEND_6) | ((color3|THEN_EXTEND_6) << 16);
        sl2->data[sl_pos] = (color2|THEN_EXTEND_6) | ((color4|THEN_EXTEND_6) << 16);
        sl_pos++;
    }

    // Pad 40 pixels on the right to center horizontally
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
