#include <pico/stdlib.h>
#include <pico/cyw43_arch.h>
#include <pico/multicore.h>
#include <hardware/flash.h>
#include <hardware/watchdog.h>
#include <hardware/resets.h>
#include "config.h"

void flash_reboot() __attribute__ ((noreturn));

// Reboot the Pico
void flash_reboot() {
    multicore_reset_core1();

    reset_block((1<<11) | (1<<10) | (1<<2));

    watchdog_enable(1, 1);
    for(;;);
}

#if 0

static uint16_t crc16_tab[256];
uint32_t crc16_initdone = 0;

static void flash_crc16_init() {
    for (uint i=0; i<256; i++) {
        uint16_t crc = 0;
        uint16_t c   = i;

        for (uint j=0; j<8; j++) {

             if ( (crc ^ c) & 0x0001 ) crc = ( crc >> 1 ) ^ 0xA001;
             else                      crc =   crc >> 1;

             c = c >> 1;
        }
        crc16_tab[i] = crc;
    }
    crc16_initdone = 1;
}

int flash_check_crc16(uint8_t *p, uint32_t size, uint8_t *c) {
    uint16_t crc = 0x0000;

    if(!crc16_initdone) flash_crc16_init();

    for(uint32_t i = 0; i < size; i++) {
        crc = (crc >> 8) ^ crc16_tab[ (crc ^ (uint16_t) *p++) & 0x00FF ];
    }

    return (crc != ((uint16_t)c[0]) | (((uint16_t)c[1]) << 8));
}

// Erase a flash sector under Apple II control
void flash_doerase() {
    uint32_t erase_offset = ((uint32_t)terminal_address) << 12;

    terminal_reg[0xE] = FLASH_STATUS_BUSY;

    if((terminal_reg[0x9] == ~terminal_reg[0x1]) && (terminal_reg[0xA] == ~terminal_reg[0x2])) {
        flash_range_erase(erase_offset, FLASH_SECTOR_SIZE);
        terminal_reg[0xE] = FLASH_STATUS_READY;
    } else {
        terminal_reg[0xE] = FLASH_STATUS_EPRM;
    }
}

// Write 4096 bytes from Apple II Memory $3000-$3FFF to flash
void flash_dowrite() {
    uint32_t write_offset = ((uint32_t)terminal_address) << 12;

    if((terminal_reg[0x9] == ~terminal_reg[0x1]) && (terminal_reg[0xA] == ~terminal_reg[0x2])) {
        if(flash_check_crc16((uint8_t*)hires_memory+0x1000, FLASH_SECTOR_SIZE, (uint8_t*)terminal_reg+0xC) != 0) {
            terminal_reg[0xE] = FLASH_STATUS_ECRC;
        } else {
            flash_range_program(write_offset, hires_memory+0x1000, FLASH_SECTOR_SIZE);
            terminal_reg[0xE] = FLASH_STATUS_READY;
        }
    } else {
        terminal_reg[0xE] = FLASH_STATUS_EPRM;
    }
}

void flash_dowork() {
    uint32_t op = flash_control;
    flash_control = FLASH_CONTROL_IDLE;

    switch(op) {
    case FLASH_CONTROL_REBOOT:
        flash_reboot();
        break;
#if 0
    case FLASH_CONTROL_ERASE:
        flash_doerase();
        break;
    case FLASH_CONTROL_UPLOAD:
        flash_dowrite();
        break;
#endif
    default:
        terminal_reg[0xE] = FLASH_STATUS_ECMD;
        break;
    }
}

#endif

