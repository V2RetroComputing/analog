#pragma once

#include <stdint.h>
#include "common/buffers.h"

#define text_memory text_p1
#define hires_memory hgr_p1
#define terminal_memory (private_memory+0xF000)

extern uint8_t character_rom[2048];
extern uint8_t terminal_character_rom[2048];

extern volatile uint32_t mono_palette;
extern volatile uint8_t terminal_row;
extern volatile uint8_t terminal_col;

#define terminal_tbcolor  apple_memory[0xC022]
#define terminal_border   apple_memory[0xC034]

#define TERMINAL_FORE       ((terminal_tbcolor>>4) & 0xf)
#define TERMINAL_BACK       (terminal_tbcolor & 0xf)
#define TERMINAL_BORDER     (terminal_border & 0xf)

