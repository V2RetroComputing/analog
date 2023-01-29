#pragma once

#include <stdint.h>
#include "common/buffers.h"

extern volatile uint32_t z80_vect;
extern volatile uint8_t z80_irq;
extern volatile uint8_t z80_nmi;
extern volatile uint8_t z80_res;
extern uint8_t z80_rom[2*1024];
#define z80_ram private_memory
extern volatile uint8_t *pcpi_reg;

#define clr_z80_stat { pcpi_reg[2] &= ~0x80; }
#define set_z80_stat { pcpi_reg[2] |= 0x80; }
#define rd_z80_stat (pcpi_reg[2] >> 7)

#define clr_6502_stat { pcpi_reg[3] &= ~0x80; }
#define set_6502_stat { pcpi_reg[3] |= 0x80; }
#define rd_6502_stat (pcpi_reg[3] >> 7)

