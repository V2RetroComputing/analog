#pragma once

#include <stdint.h>
#include "common/buffers.h"

enum {
    SOFTSW_TEXT_MODE    = 0x01,
    SOFTSW_MIX_MODE     = 0x02,
    SOFTSW_HIRES_MODE   = 0x04,
    SOFTSW_MODE_MASK    = 0x07,

    SOFTSW_PAGE_2       = 0x08,
};

#define text_memory text_p1
#define hires_memory hgr_p1
#define videx_memory (private_memory+0xF000)

extern volatile uint32_t soft_switches;
extern uint8_t character_rom[2048];
extern uint8_t terminal_rom[2048];

extern volatile uint8_t videx_crtc_reg;
extern volatile uint8_t videx_addr;

extern volatile uint8_t terminal_switches;
#define terminal_tbcolor  apple_memory[0xC022]
#define terminal_border   apple_memory[0xC034]

enum {
    TERMINAL_80COL      = 0x01,
    TERMINAL_DGR        = 0x02,
    TERMINAL_AUX_WRITE  = 0x10,
    TERMINAL_TEST       = 0x20,
    TERMINAL_IIE_REGS   = 0x40,
    TERMINAL_IIGS_REGS  = 0x80,
};

#define TERMINAL_FORE       (terminal_tbcolor & 0xf)
#define TERMINAL_BACK       ((terminal_tbcolor>>4) & 0xf)
#define TERMINAL_BORDER     (terminal_border & 0xf)

extern uint32_t flash_control;

enum {
    FLASH_STATUS_READY = 0x00,
    FLASH_STATUS_BUSY  = 0x01,
    FLASH_STATUS_ECMD  = 0xE0, // ERROR: Unknown Command
    FLASH_STATUS_EPRM  = 0xE2, // ERROR: Incorrect Parameter
    FLASH_STATUS_ECRC  = 0xE4, // ERROR: Invalid Checksum

    FLASH_CONTROL_IDLE   = 0x00000000,
    FLASH_CONTROL_ERASE  = 0x73417245,
    FLASH_CONTROL_UPLOAD = 0x64704c55,
    FLASH_CONTROL_REBOOT = 0x74426552,
};

