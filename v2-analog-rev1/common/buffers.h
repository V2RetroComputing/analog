#include <stdint.h>

extern volatile uint8_t config_memory[32];

extern volatile uint8_t apple_memory[64*1024];
extern volatile uint8_t private_memory[64*1024];

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
