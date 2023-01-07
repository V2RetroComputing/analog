#include "z80buf.h"

volatile uint32_t z80_vect = 0x000000;
volatile uint8_t z80_irq = 0;
volatile uint8_t z80_nmi = 0;
volatile uint8_t z80_res = 0;

