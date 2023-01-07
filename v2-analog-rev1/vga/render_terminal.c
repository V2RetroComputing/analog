#include <pico/stdlib.h>
#include "vgabuf.h"
#include "render.h"
#include "character_rom.h"
#include "vgaout.h"

static inline uint_fast8_t __time_critical_func(char_terminal_bits)(uint_fast8_t ch, uint_fast8_t glyph_line) {
    uint_fast8_t bits = character_rom[((uint_fast16_t)ch << 3) | glyph_line];
    if(ch & 0x80) {
        // normal character
        return bits & 0x7f;
    }

    if((bits & 0x80) == 0) {
        // inverse character
        return bits ^ 0x7f;
    } else {
        // flashing character
        return (bits ^ text_flasher_mask) & 0x7f;
    }
}


void __time_critical_func(render_terminal)() {
    vga_prepare_frame();

    render_border();

    for(int line=0; line < 24; line++) {
        render_terminal_line(line);
    }

    render_border();
}


void __time_critical_func(render_terminal_line)(unsigned int line) {
    const uint8_t *page = (const uint8_t *)videx_memory;
    const uint8_t *line_buf = page + (line * 80);

    for(uint glyph_line=0; glyph_line < 8; glyph_line++) {
        struct vga_scanline *sl = vga_prepare_scanline();
        uint sl_pos = 0;

        // Pad 40 pixels on the left to center horizontally
        while(sl_pos < 40/8) {
            sl->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
            sl_pos++;
        }

        for(uint col=0; col < 80; ) {
            // Grab 14 pixels from the next two characters
            uint32_t bits_a = char_terminal_bits(line_buf[col], glyph_line);
            col++;
            uint32_t bits_b = char_terminal_bits(line_buf[col], glyph_line);
            col++;

            uint32_t bits = (bits_a << 7) | bits_b;

            // Translate each pair of bits into a pair of pixels
            for(int i=0; i < 7; i++) {
                uint32_t pixeldata = (bits & 0x2000) ? (text_fore) : (text_back);
                pixeldata |= (bits & 0x1000) ?
                    (((uint32_t)text_fore) << 16) :
                    (((uint32_t)text_back) << 16);
                bits <<= 2;

                sl->data[sl_pos] = pixeldata;
                sl_pos++;
            }
        }

        for(uint i=0; i < 40/8; i++) {
            sl->data[sl_pos] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word
            sl_pos++;
        }

        sl->length = sl_pos;
        sl->repeat_count = 1;
        vga_submit_scanline(sl);
    }
}
