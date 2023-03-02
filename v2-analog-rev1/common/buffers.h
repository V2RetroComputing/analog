#pragma once

#include <stdint.h>

extern volatile uint8_t reset_state;

extern volatile uint8_t cardslot;
extern volatile uint32_t busactive;

extern volatile uint8_t apple_memory[64*1024];
extern volatile uint8_t private_memory[64*1024];

#define config_cmdbuf ((uint8_t*)(apple_memory+0xC0F0+(cardslot<<8)))
#define config_errbuf ((uint8_t*)(apple_memory+0xC0F8+(cardslot<<8)))

extern volatile uint8_t *text_p1;
extern volatile uint8_t *text_p2;
extern volatile uint8_t *text_p3;
extern volatile uint8_t *text_p4;
extern volatile uint8_t *hgr_p1;
extern volatile uint8_t *hgr_p2;
extern volatile uint8_t *hgr_p3;
extern volatile uint8_t *hgr_p4;
extern volatile uint8_t *baseio;
extern volatile uint8_t *slotio;
extern volatile uint8_t *slotrom;
extern volatile uint8_t *extdrom;

/* Slot 1: Grapper */
extern volatile uint8_t *slot1io;
extern volatile uint8_t *slot1rom;

/* Slot 2: Super Serial Card */
extern volatile uint8_t *slot2io;
extern volatile uint8_t *slot2rom;

/* Slot 3: 80 Column Card */
extern volatile uint8_t *slot3io;
extern volatile uint8_t *slot3rom;

/* Slot 4: PCPI Applicard */
extern volatile uint8_t *slot4io;
extern volatile uint8_t *slot4rom;

/* Slot 5: Uthernet */
extern volatile uint8_t *slot5io;
extern volatile uint8_t *slot5rom;

/* Slot 6: Disk II */
extern volatile uint8_t *slot6io;
extern volatile uint8_t *slot6rom;

/* Slot 7: ProDOS ROM Disk */
extern volatile uint8_t *slot7io;
extern volatile uint8_t *slot7rom;

/* Videx VideoTerm */
extern volatile uint8_t *videx_page;

extern volatile uint32_t soft_switches;

extern volatile uint32_t internal_flags;

#define SOFTSW_TEXT_MODE      0x00000001
#define SOFTSW_MIX_MODE       0x00000002
#define SOFTSW_HIRES_MODE     0x00000004
#define SOFTSW_MODE_MASK      0x00000007
#define SOFTSW_PAGE_2         0x00000008

// Apple IIe/c/gs softswitches
#define SOFTSW_80STORE        0x00000100
#define SOFTSW_AUX_READ       0x00000200
#define SOFTSW_AUX_WRITE      0x00000400
#define SOFTSW_AUXZP          0x00000800
#define SOFTSW_SLOT3ROM       0x00001000
#define SOFTSW_80COL          0x00002000
#define SOFTSW_ALTCHAR        0x00004000
#define SOFTSW_DGR            0x00008000

#define SOFTSW_NEWVID_MASK    0xE0
#define SOFTSW_NEWVID_SHIFT   11

#define SOFTSW_MONOCHROME     0x00010000
#define SOFTSW_LINEARIZE      0x00020000
#define SOFTSW_SHR            0x00040000

#define SOFTSW_IOUDIS         0x00080000

#define SOFTSW_SHADOW_MASK    0x7F
#define SOFTSW_SHADOW_SHIFT   20

#define SOFTSW_SHADOW_TEXT    0x00100000
#define SOFTSW_SHADOW_HGR1    0x00200000
#define SOFTSW_SHADOW_HGR2    0x00400000
#define SOFTSW_SHADOW_SHR     0x00800000
#define SOFTSW_SHADOW_AUXHGR  0x01000000
#define SOFTSW_SHADOW_ALTDISP 0x02000000
#define SOFTSW_SHADOW_IO      0x04000000

// V2 Analog specific softswitches
#define SOFTSW_TERMINAL       0x10000000
#define IFLAGS_TEST           0x20000000
#define IFLAGS_IIE_REGS       0x40000000
#define IFLAGS_IIGS_REGS      0x80000000
