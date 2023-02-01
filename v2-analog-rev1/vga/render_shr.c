#include <pico/stdlib.h>
#include "hires_color_patterns.h"
#include "hires_dot_patterns.h"
#include "vgabuf.h"
#include "render.h"
#include "vgaout.h"

extern uint16_t dhgr_palette[16];

static void render_shr_line(uint16_t line);

static uint16_t __time_critical_func(rgb444_to_rgb333)(uint16_t a) {
    return ((a & 0xe00) >> 3) | ((a & 0xe0) >> 2) | ((a & 0xe) >> 1);
}

void __time_critical_func(render_shr)() {
    vga_prepare_frame();

    render_border(40);

    for(uint line=0; line < 200; line++) {
        render_shr_line(line);
    }

    render_border(40);
}

static void __time_critical_func(render_shr_line)(uint16_t line) {
    struct vga_scanline *sl = vga_prepare_scanline();
    uint sl_pos = 0;
    uint i;

    uint8_t control = private_memory[0x9D00 + line];
    uint32_t line_palette_offset = (control & 0xF) << 5;

    volatile uint16_t *shr_palette = (volatile uint16_t*)(private_memory + 0x9E00 + line_palette_offset);
    volatile uint8_t *line_mem = (volatile uint8_t *)(private_memory + 0x2000 + (line * 160));

    // SHR is weird. Nuff said.
    uint32_t dots = 0;
    uint32_t pixeldata;
    uint16_t color_a = 0, color_b = 0;
    int j;

    i = 0;
#if 1
    if(control & 0x80) { // 640 Pixels
        while(i < 160) {
                // Load in the next 4 subpixels
                dots = line_mem[i++];

                pixeldata = (dhgr_palette[(dots >> 6) & 0x3]);
                pixeldata |= (dhgr_palette[(dots >> 4) & 0x3]) << 16;
                sl->data[sl_pos++] = pixeldata;

                pixeldata = (dhgr_palette[(dots >> 2) & 0x3]);
                pixeldata |= (dhgr_palette[(dots >> 0) & 0x3]) << 16;
                sl->data[sl_pos++] = pixeldata;
        }
#else
    if(control & 0x80) { // 640 Pixels
        while(i < 160) {
            // Load in the next 2 pixels
            dots = (line_mem[i++] & 0xff);

            color_a = ((dots >> 4) & 0xf);
            color_b = ((dots >> 0) & 0xf);

            // Consume 2 pixels
            pixeldata = rgb444_to_rgb333(shr_palette[color_a]) | THEN_EXTEND_1;
            pixeldata |= (rgb444_to_rgb333(shr_palette[color_b]) | THEN_EXTEND_1) << 16;
            sl->data[sl_pos++] = pixeldata;
        }
#endif
    } else if(control & 0x40) { // 320 Pixels w/ color fill
        while(i < 160) {
            // Load in the next 4 subpixels
            dots = (line_mem[i++] & 0xff);

            // Consume 2 pixels
            if(dots & 0xf0) {
                color_a = ((dots >> 4) & 0xf);
            } else {
                color_a = color_b;
            }
            if(dots & 0x0f) {
                color_b = ((dots >> 0) & 0xf);
            } else {
                color_b = color_a;
            }

            pixeldata = rgb444_to_rgb333(shr_palette[color_a]) | THEN_EXTEND_1;
            pixeldata |= (rgb444_to_rgb333(shr_palette[color_b]) | THEN_EXTEND_1) << 16;
            sl->data[sl_pos++] = pixeldata;
        }
    } else { // 320 Pixels
        while(i < 160) {
            // Load in the next 2 pixels
            dots = (line_mem[i++] & 0xff);

            color_a = ((dots >> 4) & 0xf);
            color_b = ((dots >> 0) & 0xf);

            // Consume 2 pixels
            pixeldata = rgb444_to_rgb333(shr_palette[color_a]) | THEN_EXTEND_1;
            pixeldata |= (rgb444_to_rgb333(shr_palette[color_b]) | THEN_EXTEND_1) << 16;
            sl->data[sl_pos++] = pixeldata;
        }
    }

    sl->length = sl_pos;
    sl->repeat_count = 1;
    vga_submit_scanline(sl);
}
