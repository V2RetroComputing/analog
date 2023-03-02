#include "buffers.h"

volatile uint32_t soft_switches = 0;
volatile uint32_t internal_flags = 0;

volatile uint8_t reset_state = 0;

volatile uint8_t cardslot = 0;
volatile uint32_t busactive = 0;

volatile uint8_t apple_memory[64*1024];
volatile uint8_t private_memory[64*1024];

volatile uint8_t *text_p1 = apple_memory + 0x0400;
volatile uint8_t *text_p2 = apple_memory + 0x0800;
volatile uint8_t *text_p3 = private_memory + 0x0400;
volatile uint8_t *text_p4 = private_memory + 0x0800;
volatile uint8_t *hgr_p1  = apple_memory + 0x2000;
volatile uint8_t *hgr_p2  = apple_memory + 0x4000;
volatile uint8_t *hgr_p3  = private_memory + 0x2000;
volatile uint8_t *hgr_p4  = private_memory + 0x4000;
volatile uint8_t *baseio  = apple_memory + 0xc000;
volatile uint8_t *slotio  = apple_memory + 0xc080;
volatile uint8_t *slotrom = apple_memory + 0xc100;
volatile uint8_t *extdrom = apple_memory + 0xc800;

/* Slot 1: Grapper */
volatile uint8_t *slot1io  = apple_memory + 0xc090;
volatile uint8_t *slot1rom = apple_memory + 0xc100;

/* Slot 2: Super Serial Card */
volatile uint8_t *slot2io  = apple_memory + 0xc0a0;
volatile uint8_t *slot2rom = apple_memory + 0xc200;

/* Slot 3: 80 Column Card */
volatile uint8_t *slot3io  = apple_memory + 0xc0b0;
volatile uint8_t *slot3rom = apple_memory + 0xc300;

/* Slot 4: PCPI Applicard */
volatile uint8_t *slot4io  = apple_memory + 0xc0c0;
volatile uint8_t *slot4rom = apple_memory + 0xc400;

/* Slot 5: Uthernet */
volatile uint8_t *slot5io  = apple_memory + 0xc0d0;
volatile uint8_t *slot5rom = apple_memory + 0xc500;

/* Slot 6: Disk II */
volatile uint8_t *slot6io  = apple_memory + 0xc0e0;
volatile uint8_t *slot6rom = apple_memory + 0xc600;

/* Slot 7: ProDOS ROM Disk */
volatile uint8_t *slot7io  = apple_memory + 0xc0f0;
volatile uint8_t *slot7rom = apple_memory + 0xc700;
