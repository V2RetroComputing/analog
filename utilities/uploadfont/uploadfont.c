// cl65 -t apple2 --start-addr 0x4000 uploadfont.c

#include <stdio.h>
#include <conio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "../common/picopal.h"

uint16_t cardslot = 3;
uint8_t *cfgbuf;
uint8_t *fsbuf;
uint8_t *font = (uint8_t *)0x2000;

void paint_backdrop(char *str) {
    int w, i;

    w = strlen(str);
    if(w > 20) {
        w = 20;
    }

    clrscr();
    gotoy(0); gotox(1);
    cputs("V2 Analog");

    gotoy(0); gotox(39 - w);

    if((w == 19) && str[19])
        w = 16;

    for(i = 0; i < w; i++) {
        cputc(str[i]);
    }
    if((i == 17) && str[i])
        cputs("...");

    gotoy(1); gotox(0);
    cputs("----------------------------------------");

    gotoy(22); gotox(0);
    cputs("----------------------------------------");

    gotoy(23); gotox(4);
    cputs("https://www.v2retrocomputing.com");
}

void message(char *str) {
    int w, x, i, c;

    if(strlen(str) > 34) {
        w = 34;
    } else {
        w = strlen(str);
    }

    x = 20 - ((w+6)/2);

    gotoy(9); gotox(x);
    cputs(" +-");
    for(i = 0; i < (w); i++) {
        cputc('-');
    }
    cputs("-+ ");

    gotoy(10); gotox(x);
    cputs(" ! ");
    for(i = 0; i < w; i++) {
        cputc(' ');
    }
    cputs(" ! ");

    gotoy(11); gotox(x);
    cputs(" ! ");
    for(c = 0; c < w; c++) {
        cputc(str[c]);
    }
    cputs(" ! ");

    gotoy(12); gotox(x);
    cputs(" ! ");
    for(i = 0; i < w; i++) {
        cputc(' ');
    }
    cputs(" ! ");

    gotoy(13); gotox(x);
    cputs(" ! ");
    for(i = 0; i < ((w-4)/2); i++) {
        cputc(' ');
    }
    cputs("[OK]");
    i += 4;
    for(; i < w; i++) {
        cputc(' ');
    }
    cputs(" ! ");

    gotoy(14); gotox(x);
    cputs(" ! ");
    for(i = 0; i < w; i++) {
        cputc(' ');
    }
    cputs(" ! ");

    gotoy(15); gotox(x);
    cputs(" +-");
    for(i = 0; i < w; i++) {
        cputc('-');
    }
    cputs("-+ ");

    for(;;) {
        switch(cgetc()) {
        case 0x0A:
        case 0x0D:
        case 0x1B:
        case 'O':
        case 'o':
            return;
        }
    }
}

int confirm(char *str) {
    int w, x, i, c;

    if(strlen(str) > 34) {
        w = 34;
    } else {
        w = strlen(str);
    }

    x = 20 - ((w+6)/2);

    gotoy(9); gotox(x);
    cputs(" +-");
    for(i = 0; i < (w); i++) {
        cputc('-');
    }
    cputs("-+ ");

    gotoy(10); gotox(x);
    cputs(" ! ");
    for(i = 0; i < w; i++) {
        cputc(' ');
    }
    cputs(" ! ");

    gotoy(11); gotox(x);
    cputs(" ! ");
    for(c = 0; c < w; c++) {
        cputc(str[c]);
    }
    cputs(" ! ");

    gotoy(12); gotox(x);
    cputs(" ! ");
    for(i = 0; i < w; i++) {
        cputc(' ');
    }
    cputs(" ! ");

    gotoy(13); gotox(x);
    cputs(" ! ");
    for(i = 0; i < ((w-12)/2); i++) {
        cputc(' ');
    }
    cputs("[YES] / [NO]");
    i += 12;
    for(; i < w; i++) {
        cputc(' ');
    }
    cputs(" ! ");

    gotoy(14); gotox(x);
    cputs(" ! ");
    for(i = 0; i < w; i++) {
        cputc(' ');
    }
    cputs(" ! ");

    gotoy(15); gotox(x);
    cputs(" +-");
    for(i = 0; i < w; i++) {
        cputc('-');
    }
    cputs("-+ ");


    for(;;) {
        switch(cgetc()) {
        case 0x0A:
        case 0x0D:
        case 'Y':
        case 'y':
            return 1;
        case 0x1B:
        case 'N':
        case 'n':
            return 0;
        }
    }
}

void cfg_cmd(char *cmd) {
    int i = 0;
    uint16_t delay;

    while(i < 31) {
        if(*cmd) {
            cfgbuf[i++] = *cmd++;
        } else {
            cfgbuf[i++] = 0x00;
        }
    }
    cfgbuf[31] = 0x00;

    delay = 0x1fff;
    while(delay > 0) delay--;
}


int prompt_slot(void) {
    int c;
    paint_backdrop("Font Upload");

    gotoy(11); gotox(2);
    cputs("Which slot is the card installed in?");
    gotoy(13); gotox(14);
    printf("Default = %i", cardslot);

    for(;;) {
        c = cgetc();
        if((c >= '1') && (c <= '7')) {
            cardslot = c - '0';
            return 1;
        } else if((c == 0x0A) || (c == 0x0D)) {
            return 1;
        } else if(c == 0x1B) {
            return 0;
        }
    }
}

void main (void) {
    int paint_menu = 1;
    uint16_t i;

    if(!prompt_slot()) {
        return;
    }

    cfgbuf = (uint8_t *)CFG_BUFFER;
    fsbuf = (uint8_t *)FS_BUFFER;

    paint_backdrop("Please Wait");
    gotoy(11); gotox(13);
    cputs("Uploading font,");
    gotoy(12); gotox(8);
    cputs("your screen may flicker.");

    cfg_cmd("BANK=SAVE");
    cfg_cmd("BANK=FONT0");
    for(i = 0; i < 0x200; i++) {
        FS_BUFFER[i] = font[i];
    }
    cfg_cmd("BANK=FONT1");
    for(i = 0; i < 0x200; i++) {
        FS_BUFFER[i] = font[i+512];
    }
    cfg_cmd("BANK=FONT2");
    for(i = 0; i < 0x200; i++) {
        FS_BUFFER[i] = font[i+1024];
    }
    cfg_cmd("BANK=FONT3");
    for(i = 0; i < 0x200; i++) {
        FS_BUFFER[i] = font[i+1536];
    }
    cfg_cmd("BANK=RESTORE");

    clrscr();
    puts("Done.\n");
}
