#include "vgabuf.h"

volatile uint32_t mono_palette = 0;

// The currently programmed character generator ROM for text mode
uint8_t character_rom[2048];
uint8_t terminal_character_rom[2048];

volatile uint8_t terminal_row = 0;
volatile uint8_t terminal_col = 0;
