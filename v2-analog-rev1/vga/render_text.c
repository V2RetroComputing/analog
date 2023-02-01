#include <pico/stdlib.h>
#include "vgabuf.h"
#include "render.h"
#include "character_rom.h"
#include "vgaout.h"

//#define PAGE2SEL (!(soft_switches & SOFTSW_80STORE) && (soft_switches & SOFTSW_PAGE_2))
#define PAGE2SEL ((soft_switches & (SOFTSW_80STORE | SOFTSW_PAGE_2)) == SOFTSW_PAGE_2)

uint_fast32_t text_flasher_mask = 0;
static uint64_t next_flash_tick = 0;

void update_text_flasher() {
    uint64_t now = time_us_64();
    if(now > next_flash_tick) {
        text_flasher_mask ^= 0x7f;

        switch(current_machine) {
        default:
        case MACHINE_II:
        case MACHINE_PRAVETZ:
            next_flash_tick = now + 125000u;
            break;
        case MACHINE_IIE:
        case MACHINE_IIGS:
            next_flash_tick = now + 250000u;
            break;
        }
    }
}


static inline uint_fast8_t __time_critical_func(char_text_bits)(uint_fast8_t ch, uint_fast8_t glyph_line) {
    uint_fast8_t bits = character_rom[((uint_fast16_t)ch << 3) | glyph_line] & 0x7f;

    if((soft_switches & SOFTSW_ALTCHAR) || (ch & 0x80)) {
        // normal character
        return bits;
    }

    if(ch & 0x40) {
        // flashing character
        return (bits ^ text_flasher_mask);
    }

    // inverse character
    return bits;
}

void __time_critical_func(render_text)() {
    for(uint line=0; line < 24; line++) {
        if(soft_switches & SOFTSW_80COL) {
            render_text80_line(PAGE2SEL, line);
        } else {
            render_text40_line(PAGE2SEL, line);
        }
    }
}

void __time_critical_func(render_text40_line)(bool p2, unsigned int line) {
    const uint8_t *page = (const uint8_t *)(p2 ? text_p2 : text_p1);
    const uint8_t *line_buf = (const uint8_t *)(page + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40));

    for(uint glyph_line=0; glyph_line < 8; glyph_line++) {
        struct vga_scanline *sl = vga_prepare_scanline();
        uint sl_pos = 0;

        // Pad 40 pixels on the left to center horizontally
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word

        for(uint col=0; col < 40; ) {
            // Grab 14 pixels from the next two characters
            uint32_t bits_a = char_text_bits(line_buf[col], glyph_line);
            col++;
            uint32_t bits_b = char_text_bits(line_buf[col], glyph_line);
            col++;

            uint32_t bits = (bits_a << 7) | bits_b;

            // Translate each pair of bits into a pair of pixels
            for(int i=0; i < 7; i++) {
                uint32_t pixeldata = (bits & 0x2000) ? (text_back|THEN_EXTEND_1) : (text_fore|THEN_EXTEND_1);
                pixeldata |= (bits & 0x1000) ?
                    ((uint32_t)text_back|THEN_EXTEND_1) << 16 :
                    ((uint32_t)text_fore|THEN_EXTEND_1) << 16;
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

void __time_critical_func(render_text80_line)(bool p2, unsigned int line) {
    const uint8_t *page_a = (const uint8_t *)(p2 ? text_p2 : text_p1);
    const uint8_t *page_b = (const uint8_t *)(p2 ? text_p4 : text_p3);
    const uint8_t *line_buf_a = page_a + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40);
    const uint8_t *line_buf_b = page_b + ((line & 0x7) << 7) + (((line >> 3) & 0x3) * 40);

    for(uint glyph_line=0; glyph_line < 8; glyph_line++) {
        struct vga_scanline *sl = vga_prepare_scanline();
        uint sl_pos = 0;

        // Pad 40 pixels on the left to center horizontally
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_7) | ((text_border|THEN_EXTEND_7) << 16); // 16 pixels per word
        sl->data[sl_pos++] = (text_border|THEN_EXTEND_3) | ((text_border|THEN_EXTEND_3) << 16); // 8 pixels per word

        for(uint col=0; col < 40; ) {
            // Grab 14 pixels from the next two characters
            uint32_t bits_a = char_text_bits(line_buf_a[col], glyph_line);
            uint32_t bits_b = char_text_bits(line_buf_b[col], glyph_line);
            col++;

            uint32_t bits = (bits_b << 7) | bits_a;

            // Translate each pair of bits into a pair of pixels
            for(int i=0; i < 7; i++) {
                uint32_t pixeldata = (bits & 0x2000) ? (text_back) : (text_fore);
                pixeldata |= (bits & 0x1000) ?
                    ((uint32_t)text_back) << 16 :
                    ((uint32_t)text_fore) << 16;
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

