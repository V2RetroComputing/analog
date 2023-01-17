#pragma once

#include <stdint.h>

// Uncomment to enable test patter generator
#define RENDER_TEST_PATTERN

extern uint16_t lores_palette[16];
extern uint16_t text_fore, text_back, text_border;

extern void render_init();
extern void render_loop();

extern void render_testpattern();
extern void render_test_init();
extern void render_about_init();

extern void update_text_flasher();
extern void render_text();
extern void render_text40_line(bool p2, unsigned int line);
extern void render_text80_line(bool p2, unsigned int line);

extern void render_videx();
extern void render_videx_line(unsigned int line);

extern void render_border();

extern void render_lores();
extern void render_mixed_lores();

extern void render_hires();
extern void render_mixed_hires();

extern void render_dhgr();
extern void render_mixed_dhgr();

extern void render_dgr();
extern void render_mixed_dgr();

extern void render_shr();

extern uint_fast32_t text_flasher_mask;

extern void flash_dowork();

