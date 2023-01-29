// cl65 -t apple2 --start-addr 0x4000 uploadfont.c

#include <stdio.h>
#include <conio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#include "../common/picopal.h"

uint16_t cardslot = 3;
uint8_t *cfgbuf;

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
    paint_backdrop("Mode Switch");

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
    int c;

    if(!prompt_slot()) {
        return;
    }

    cfgbuf = (uint8_t *)(0xC0E0 | (cardslot << 8));

    paint_backdrop("Mode Switch");
    gotoy(11); gotox(4);
    cputs("1. VGA Card");
    gotoy(12); gotox(4);
    cputs("2. PCPI Applicard");
    gotoy(13); gotox(4);
    cputs("3. Serial");
    gotoy(14); gotox(4);
    cputs("4. Parallel");
    gotoy(15); gotox(4);
    cputs("5. Card Diagnostic");
    gotoy(18); gotox(4);
    cputs("8. Save Default");
    gotoy(19); gotox(4);
    cputs("9. Quit");

    for(;;) {
        c = cgetc();
        switch(c) {
        case '1':
            cfg_cmd("MODE=VGA");
            break;
        case '2':
            cfg_cmd("MODE=Z80");
            break;
        case '3':
            cfg_cmd("MODE=SERIAL");
            break;
        case '4':
            cfg_cmd("MODE=PARALLEL");
            break;
        case '5':
            cfg_cmd("MODE=DIAG");
            break;
        case '8':
            cfg_cmd("WRITE_CONFIG");
            break;
        case '9':
            clrscr();
            puts("Done.\n");
            return;
        }
    }

}
