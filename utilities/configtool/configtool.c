// cl65 -t apple2 --start-addr 0x4000 uploadfont.c

#include <stdio.h>
#include <conio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

#define PICOPAL_SLOT 3
#include "../common/picopal.h"

void cardconfig(void) {
    puts("1. Firmware Mode\n");
    if(v2mode == MODE_SERIAL) {
        puts("2. Device Mux\n");
        puts("3. Baudrate\n");
    }
    if(v2mode == MODE_PARALLEL) {
        puts("2. Device Mux\n");
        if(muxmode == MUX_PRINTER) {
            puts("3. Host\n");
            puts("4. Port\n");
        }
    }
}

void wificonfig(void) {
    puts("1. WIFI Mode\n");
    puts("2. Network Name\n");
    puts("3. Pre-Shared Key\n");
}

void main (void) {
    unsigned char *config = (unsigned char *)0x2000;
    int i;

    puts("1. Card Configuration\n");
    puts("4. WIFI Configuration\n");

    PICOPAL_ALSB = 0x00;
    PICOPAL_AMSB = 0x00;

    for(i = 0; i < 0x800; i++) {
        PICOPAL_WRFONT = font[i];
    }

    puts("Done.\n");
}
