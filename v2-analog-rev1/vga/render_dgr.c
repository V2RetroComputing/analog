#include <pico/stdlib.h>
#include "vgabuf.h"
#include "render.h"
#include "vgaout.h"


extern uint16_t lores_palette[16];

static void render_dgr_line(uint line);

void __time_critical_func(render_dgr)() {
    vga_prepare_frame();

    // Skip 48 lines to center vertically
    struct vga_scanline *skip_sl = vga_prepare_scanline();
    for(int i=0; i < 48; i++) {
        skip_sl->data[i] = (uint32_t)THEN_WAIT_HSYNC << 16;
    }
    skip_sl->length = 48;
    vga_submit_scanline(skip_sl);

    for(uint line=0; line < 24; line++) {
        render_dgr_line(line);
    }
}


void __time_critical_func(render_mixed_dgr)() {
    vga_prepare_frame();

    render_border();

    for(uint line=0; line < 20; line++) {
        render_dgr_line(line);
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


static void __time_critical_func(render_dgr_line)(uint line) {
    // Construct two scanlines for the two different colored cells at the same time
    struct vga_scanline *sl1 = vga_prepare_scanline();
    struct vga_scanline *sl2 = vga_prepare_scanline();
    uint sl_pos = 0;
    uint i;

    const uint8_t *page = (const uint8_t *)text_memory;
    const uint8_t *line_buf = page + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40);

    // Pad 40 pixels on the left to center horizontally
    while(sl_pos < 40/8) {
        sl1->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl2->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
        sl_pos++;
    }

    for(i=0; i < 40; i++) {
        uint32_t color1 = lores_palette[line_buf[i | (1 << 10)] & 0xf];
        uint32_t color2 = lores_palette[(line_buf[i | (1 << 10)] >> 4) & 0xf];
        uint32_t color3 = lores_palette[line_buf[i] & 0xf];
        uint32_t color4 = lores_palette[(line_buf[i] >> 4) & 0xf];

        // Each double lores pixel is 7 double hires pixels, or 7 VGA pixels wide
        sl1->data[sl_pos] = (color1|THEN_EXTEND_6) | ((color3|THEN_EXTEND_6) << 16);
        sl2->data[sl_pos] = (color2|THEN_EXTEND_6) | ((color4|THEN_EXTEND_6) << 16);
        sl_pos++;
    }

    for(i=0; i < 40/8; i++) {
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
