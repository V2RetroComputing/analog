#include "vgabuf.h"


// Shadow copy of the Apple soft-switches
volatile uint32_t soft_switches = 0;

// The currently programmed character generator ROM for text mode
uint8_t character_rom[2048];

volatile uint8_t videx_crtc_reg;

volatile uint8_t terminal_switches;
