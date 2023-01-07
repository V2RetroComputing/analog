// cl65 -t apple2 --start-addr 0x4000 uploadfont.c

#include <stdio.h>
#include <conio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define PICOPAL_SLOT 3
#include "../common/picopal.h"

void main (void)
{
    unsigned char *font = (unsigned char *)0x2000;
    int i;

    puts("Loading font to PicoPal in slot 3...");

    PICOPAL_ALSB = 0x00;
    PICOPAL_AMSB = 0x00;

    for(i = 0; i < 0x800; i++) {
        PICOPAL_WRFONT = font[i];
    }

    puts("Done.\n");
}
