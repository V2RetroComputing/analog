#include <pico/stdlib.h>
#include <pico/multicore.h>
#include "common/config.h"
#include "z80/businterface.h"
#include "z80/z80buf.h"
#include "z80/z80rom.h"

volatile uint8_t rom_shadow = 1;
volatile uint8_t ram_bank = 0;
volatile uint8_t ram_common = 0;

#define Z80break (z80_res || (current_mode != MODE_APPLICARD) || (config_cmdbuf[7] == 0))

uint8_t cpu_in(uint16_t address) {
    uint8_t rv = 0;
    switch(address & 0xe0) {
    case 0x00: // Write Data to 6502
        rv = pcpi_reg[0];
        break;
    case 0x20: // Read Data from 6502
        clr_z80_stat;
        rv = pcpi_reg[1];
        //printf("I%01X:%02X\r\n", (address >> 4), rv);
        break;
    case 0x40: // Status Port
        if(rd_z80_stat)
            rv |= 0x80;
        if(rd_6502_stat)
            rv |= 0x01;
        break;
    case 0x60:
        break;
    }
    return rv;
}

void cpu_out(uint16_t address, uint8_t value) {
    switch(address & 0xe0) {
    case 0x00: // Write Data to 6502
        //printf("O%01X:%02X\r\n", (address >> 4), value);
        pcpi_reg[0] = value;
        set_6502_stat;
        break;
    case 0x60:
        rom_shadow = (value & 1);
        break;
    case 0xC0:
        ram_bank = (value >> 1) & 7;
        ram_common = (value >> 6) & 1;
        break;
    }
}

uint8_t _RamRead(uint16_t address) {
    if((rom_shadow & 1) && (address < 0x8000))
        return z80_rom[address & 0x7ff];

    if((address > 0xE000) && (ram_common)) {
        return z80_ram[address];
    }

    if(ram_bank) {
        return 0xff;
    }

    return z80_ram[address];
}

void _RamWrite(uint16_t address, uint8_t value) {
    if((rom_shadow & 1) && (address < 0x8000))
        return;

    if((address > 0xE000) && (ram_common)) {
        z80_ram[address] = value;
        return;
    }

    if(ram_bank) {
        return;
    }

    z80_ram[address] = value;
}

#include "z80cpu.h"

void z80main() {
    z80_res = 1;

    while(current_mode == MODE_APPLICARD) {
        config_handler();
        if(cardslot != 0) {
            if(z80_res) {
                rom_shadow = 1;
                ram_bank = 0;
                ram_common = 0;

                z80_nmi = 0;
                z80_irq = 0;
                z80_res = 0;

                // 6502 -> Z80
                clr_z80_stat;

                // Z80 -> 6502
                clr_6502_stat;

                Z80reset();
            }

            Z80run();
        }
    }
}

