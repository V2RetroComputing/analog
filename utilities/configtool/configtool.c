// cl65 -t apple2 --start-addr 0x4000 uploadfont.c

#include <stdio.h>
#include <conio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

#include "../common/picopal.h"

volatile uint16_t cardslot = 3;

#define BASL      0x28

#define menutop 3
#define menuleft 2

v2mode_t v2mode = MODE_VGACARD;

serialmux_t serialmux = SERIAL_PRINTER;
uint16_t baud = 19200;

usbmux_t usbmux = USB_GUEST_CDC;
compat_t machine = MACHINE_APPLE_IIGS;
wifimode_t wifimode = WIFI_AP;
char wifi_ssid[32] = "V2RetroNet";
char wifi_psk[32] = "Analog";
char wifi_address[32] = "DHCP";

char jd_host[32] = "192.168.0.1";
uint16_t jd_port = 9100;

char config_data[1024];
uint16_t config_size = 0;
uint8_t *config_cmdbuf;
uint8_t *config_errbuf;
uint8_t *upload_buf = (uint8_t *)(0xC800);

char ERR_READY[8]  = { 'R', 'E', 'A', 'D', 'Y',   0,   0,   0 };
char ERR_EOF[8]    = { 'E', 'O', 'F',   0,   0,   0,   0,   0 };
char ERR_NOFILE[8] = { 'E', 'O', 'F',   0,   0,   0,   0,   0 };

#define CARD_TIMEOUT 0x3fff
uint16_t timeout = CARD_TIMEOUT;

void default_config() {
    v2mode = MODE_VGACARD;
    serialmux = SERIAL_PRINTER;
    baud = 19200;
    usbmux = USB_GUEST_CDC;

    wifimode = WIFI_AP;
    strcpy(wifi_ssid, "V2RetroNet");
    strcpy(wifi_psk, "Analog");
    strcpy(wifi_address, "DHCP");

    strcpy(jd_host, "192.168.0.1");
    jd_port = 9100;
    machine = MACHINE_AUTO;
}

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

void cfg_cmd0(char *cmd) {
    char cfg_temp[32];

    memset(cfg_temp, 0, 32);
    strncpy(cfg_temp, cmd, 32);
    memcpy(config_cmdbuf, cfg_temp, 32);
}

void cfg_cmd1(char *cmd, uint16_t param0) {
    uint8_t cfg_temp[32];

    memset(cfg_temp, 0, 32);
    memcpy(cfg_temp, cmd, 2);
    cfg_temp[2] = param0 & 0xFF;
    cfg_temp[3] = (param0 >> 8) & 0xFF;
    memcpy(config_cmdbuf, cfg_temp, 32);
}

void cfg_cmd2(char *cmd, uint16_t param0, uint16_t param1) {
    uint8_t cfg_temp[32];

    memset(cfg_temp, 0, 32);
    memcpy(cfg_temp, cmd, 2);
    cfg_temp[2] = param0 & 0xFF;
    cfg_temp[3] = (param0 >> 8) & 0xFF;
    cfg_temp[4] = param1 & 0xFF;
    cfg_temp[5] = (param1 >> 8) & 0xFF;
    memcpy(config_cmdbuf, cfg_temp, 32);
}

void parse_config(char *buffer) {
    if(!memcmp("C=", buffer, 2)) {
        if(!strcmp("DIAG", buffer+2)) {
            v2mode = MODE_DIAG;
        } else if(!strcmp("FS", buffer+2)) {
            v2mode = MODE_FS;
        } else if(!strcmp("VGA", buffer+2)) {
            v2mode = MODE_VGACARD;
        } else if(!strcmp("Z80", buffer+2)) {
            v2mode = MODE_APPLICARD;
        } else if(!strcmp("SER", buffer+2)) {
            v2mode = MODE_SERIAL;
        } else if(!strcmp("PAR", buffer+2)) {
            v2mode = MODE_PARALLEL;
        } else if(!strcmp("MIDI", buffer+2)) {
            v2mode = MODE_MIDI;
        } else if(!strcmp("SNES", buffer+2)) {
            v2mode = MODE_SNESMAX;
        } else if(!strcmp("NET", buffer+2)) {
            v2mode = MODE_ETHERNET;
        }
    } else if(!memcmp("H=", buffer, 2)) {
        if(!strcmp("AUTO", buffer+2)) {
            machine = MACHINE_AUTO;
        } else if(!strcmp("II", buffer+2)) {
            machine = MACHINE_APPLE_II;
        } else if(!strcmp("IIE", buffer+2)) {
            machine = MACHINE_APPLE_IIE;
        } else if(!strcmp("IIGS", buffer+2)) {
            machine = MACHINE_APPLE_IIGS;
        }
    } else if(!memcmp("M=", buffer, 2)) {
        if(!strcmp("USB", buffer+2)) {
            serialmux = SERIAL_USB;
        } else if(!strcmp("WIFI", buffer+2)) {
            serialmux = SERIAL_WIFI;
        } else if(!strcmp("PRN", buffer+2)) {
            serialmux = SERIAL_PRINTER;
        } else if(!strcmp("LOOP", buffer+2)) {
            serialmux = SERIAL_LOOP;
        }
    } else if(!memcmp("U=", buffer, 2)) {
        if(!strcmp("CG", buffer+2)) {
            usbmux = USB_GUEST_CDC;
        } else if(!strcmp("CH", buffer+2)) {
            usbmux = USB_HOST_CDC;
        }
    } else if(!memcmp("W=", buffer, 2)) {
        if(!strcmp("C", buffer+2)) {
            wifimode = WIFI_CLIENT;
        } else if(!strcmp("A", buffer+2)) {
            wifimode = WIFI_AP;
        }
    } else if(!memcmp("WS=", buffer, 3)) {
        strncpy(wifi_ssid, buffer+3, 25);
    } else if(!memcmp("WP=", buffer, 3)) {
        strncpy(wifi_psk, buffer+3, 26);
    } else if(!memcmp("IP=", buffer, 3)) {
        strncpy(wifi_address, buffer+3, 16);
    } else if(!memcmp("JDH=", buffer, 4)) {
        strncpy(jd_host, buffer+4, 16);
    } else if(!memcmp("JDP=", buffer, 4)) {
        jd_port = strtoul(buffer+4, NULL, 0);
    } else if(!memcmp("SB=", buffer, 3)) {
        baud = strtoul(buffer+3, NULL, 0);
    }
}

void build_config(void) {
    char *config_temp = config_data;

    memset(config_data, 0, sizeof(config_data));
    switch(v2mode) {
    case MODE_FS:
        strcpy(config_temp, "C=FS");
        config_temp += 32;
        break;
    case MODE_VGACARD:
        strcpy(config_temp, "C=VGA");
        config_temp += 32;
        break;
    case MODE_APPLICARD:
        strcpy(config_temp, "C=Z80");
        config_temp += 32;
        break;
    case MODE_SERIAL:
        strcpy(config_temp, "C=SER");
        config_temp += 32;
        break;
    case MODE_PARALLEL:
        strcpy(config_temp, "C=PAR");
        config_temp += 32;
        break;
    case MODE_MIDI:
        strcpy(config_temp, "C=MIDI");
        config_temp += 32;
        break;
    case MODE_SNESMAX:
        strcpy(config_temp, "C=SNES");
        config_temp += 32;
        break;
    case MODE_ETHERNET:
        strcpy(config_temp, "C=NET");
        config_temp += 32;
        break;
    }
    switch(machine) {
    case MACHINE_AUTO:
        strcpy(config_temp, "H=AUTO");
        config_temp += 32;
        break;
    case MACHINE_APPLE_II:
        strcpy(config_temp, "H=II");
        config_temp += 32;
        break;
    case MACHINE_APPLE_IIE:
        strcpy(config_temp, "H=IIE");
        config_temp += 32;
        break;
    case MACHINE_APPLE_IIGS:
        strcpy(config_temp, "H=IIGS");
        config_temp += 32;
        break;
    }
    switch(serialmux) {
    case SERIAL_USB:
        strcpy(config_temp, "M=USB");
        config_temp += 32;
        break;
    case SERIAL_WIFI:
        strcpy(config_temp, "M=WIFI");
        config_temp += 32;
        break;
    case SERIAL_PRINTER:
        strcpy(config_temp, "M=PRN");
        config_temp += 32;
        break;
    case SERIAL_LOOP:
        strcpy(config_temp, "M=LOOP");
        config_temp += 32;
        break;
    }
    switch(usbmux) {
    case USB_GUEST_CDC:
        strcpy(config_temp, "U=CG");
        config_temp += 32;
        break;
    case USB_HOST_CDC:
        strcpy(config_temp, "U=CH");
        config_temp += 32;
        break;
    }

    switch(wifimode) {
    case WIFI_CLIENT:
        strcpy(config_temp, "W=C");
        config_temp += 32;
        break;
    case WIFI_AP:
        strcpy(config_temp, "W=A");
        config_temp += 32;
        break;
    }

    strcpy(config_temp, "WS=");
    strncpy(config_temp+3, wifi_ssid, 24);
    config_temp += 32;

    strcpy(config_temp, "WP=");
    strncpy(config_temp+3, wifi_psk, 24);
    config_temp += 32;

    strcpy(config_temp, "IP=");
    strncpy(config_temp+3, wifi_address, 16);
    config_temp += 32;

    strcpy(config_temp, "JDH=");
    strncpy(config_temp+4, jd_host, 16);
    config_temp += 32;

    sprintf(config_temp, "JDP=%d", jd_port);
    config_temp += 32;

    sprintf(config_temp, "SB=%d", baud);
    config_temp += 32;
}

void read_config(void) {
    int i;
    uint8_t dummy;
    timeout = CARD_TIMEOUT;

    // Release the $C800 region from any other card
    dummy = *(uint8_t*)0xCFFF;

    // Ask card to upload it's config to the Apple II host.
    // We only support Page0 in this version of the utility.
    cfg_cmd1("UC", 0);
    while(memcmp(config_errbuf, ERR_READY, 8) && (timeout > 0)) {
        timeout--;
    }
    if(timeout == 0) {
        // Release the $C800 region for use by other cards
        dummy = *(uint8_t*)0xCFFF;

        message("Communication Timeout");
        default_config();
        return;
    } else {
        memcpy(config_data, upload_buf, 1024);

        // Release the $C800 region for use by other cards
        dummy = *(uint8_t*)0xCFFF;
    }

    for(i = 0; i < sizeof(config_data); i+=32) {
        if(config_data[i] != 0)
            parse_config(config_data+i);
    }
}

int write_config(void) {
    uint8_t dummy;
    timeout = CARD_TIMEOUT;
    
    build_config();

    paint_backdrop("Please Wait");
    gotoy(11); gotox(10);
    cputs("Saving configuration,");
    gotoy(12); gotox(8);
    cputs("your screen may flicker.");

    // Release the $C800 region from any other card
    dummy = *(uint8_t*)0xCFFF;

    // Ask card to download it's config from the Apple II host.
    // We only support Page0 in this version of the utility.
    cfg_cmd2("DC", (uint16_t)config_data, 0);    
    while(memcmp(config_errbuf, ERR_READY, 8) && (timeout > 0)) {
        timeout--;
    }

    // Release the $C800 region for use by other cards
    dummy = *(uint8_t*)0xCFFF;

    return 1;
}

void apply_config(void) {
    uint8_t dummy;
    timeout = CARD_TIMEOUT;

    paint_backdrop("Please Wait");
    gotoy(10); gotox(13);
    cputs("Rebooting card");
    gotoy(11); gotox(9);
    cputs("to apply configuration,");
    gotoy(12); gotox(8);
    cputs("your screen may flicker.");

    // Release the $C800 region from any other card
    dummy = *(uint8_t*)0xCFFF;

    // Ask card to reboot.
    cfg_cmd0("REBOOT");

    // Release the $C800 region for use by other cards
    dummy = *(uint8_t*)0xCFFF;

    while(timeout--);
}

int format_card(void) {
    uint8_t dummy;
    timeout = CARD_TIMEOUT;

    paint_backdrop("Please Wait");
    gotoy(11); gotox(13);
    cputs("Formatting card,");
    gotoy(12); gotox(8);
    cputs("your screen may flicker.");

    // Release the $C800 region from any other card
    dummy = *(uint8_t*)0xCFFF;

    // Ask card to download it's config from the Apple II host.
    // We only support Page0 in this version of the utility.
    cfg_cmd0("FORMAT");
    while(memcmp(config_errbuf, ERR_READY, 8) && (timeout > 0)) {
        timeout--;
    }

    // Release the $C800 region for use by other cards
    dummy = *(uint8_t*)0xCFFF;

    return 1;
}

void mode_menu(void) {
    int paint_menu = 1;

    for(;;) {
        if(paint_menu) {
            paint_backdrop("Firmware Mode");

            gotoy(menutop+0); gotox(menuleft);
            cputs("1. VGA Video");
            gotoy(menutop+2); gotox(menuleft);
            cputs("2. PCPI Applicard");
            gotoy(menutop+4); gotox(menuleft);
#if 0
            cputs("3. Serial");
            gotoy(menutop+6); gotox(menuleft);
            cputs("4. Parallel");
#endif

            gotoy(menutop+17); gotox(menuleft);
            cputs("9. Back to Card Options");
            paint_menu = 0;
        }

        switch(cgetc()) {
        case '1':
            v2mode = MODE_VGACARD;
            return;
        case '2':
            v2mode = MODE_APPLICARD;
            return;
#if 0
        case '3':
            v2mode = MODE_SERIAL;
            return;
        case '4':
            v2mode = MODE_PARALLEL;
            return;
#endif
        case 0x1B:
        case '9':
            return;
        }
    }
}

void mux_menu(void) {
    int paint_menu = 1;

    for(;;) {
        if(paint_menu) {
            paint_backdrop("Serial Mux");

            gotoy(menutop+0); gotox(menuleft);
            cputs("1. Printer");
            gotoy(menutop+2); gotox(menuleft);
            cputs("2. USB");

            if(v2mode == MODE_SERIAL) {
                gotoy(menutop+4); gotox(menuleft);
                cputs("3. WiFi Modem");
                gotoy(menutop+6); gotox(menuleft);
                cputs("4. Loopback");
            }

            gotoy(menutop+17); gotox(menuleft);
            cputs("9. Back to Card Options");
        }

        switch(cgetc()) {
        case '1':
            serialmux = SERIAL_PRINTER;
            return;
        case '2':
            serialmux = SERIAL_USB;
            return;
        case '3':
            if(v2mode == MODE_SERIAL) {
                serialmux = SERIAL_WIFI;
                return;
            }
            break;
        case '4':
            if(v2mode == MODE_SERIAL) {
                serialmux = SERIAL_LOOP;
                return;
            }
            break;
        case 0x1B:
        case '9':
            return;
        }
    }
}

void baud_menu(void) {
    int paint_menu = 1;

    for(;;) {
        if(paint_menu) {
            paint_backdrop("Baudrate");

            gotoy(menutop+0); gotox(menuleft);
            cputs("1. 75");
            gotoy(menutop+2); gotox(menuleft);
            cputs("2. 300");
            gotoy(menutop+4); gotox(menuleft);
            cputs("3. 600");
            gotoy(menutop+6); gotox(menuleft);
            cputs("4. 1200");
            gotoy(menutop+8); gotox(menuleft);
            cputs("5. 2400");
            gotoy(menutop+10); gotox(menuleft);
            cputs("6. 4800");
            gotoy(menutop+12); gotox(menuleft);
            cputs("7. 9600");
            gotoy(menutop+14); gotox(menuleft);
            cputs("8. 19200");
            gotoy(menutop+17); gotox(menuleft);
            cputs("9. Back to Card Options");
            paint_menu = 0;
        }

        switch(cgetc()) {
        case '1':
            baud = 75;
            return;
        case '2':
            baud = 300;
            return;
        case '3':
            baud = 600;
            return;
        case '4':
            baud = 1200;
            return;
        case '5':
            baud = 2400;
            return;
        case '6':
            baud = 4800;
            return;
        case '7':
            baud = 9600;
            return;
        case '8':
            baud = 19200;
            return;
        case 0x1B:
        case '9':
            return;
        }
    }
}


void jdhost_edit(void) {
    char temp_host[32];
    int c, l;
    int paint_menu = 1;
    
    strcpy(temp_host, jd_host);
    l = strlen(temp_host);

    for(;;) {
        if(paint_menu) {
            paint_backdrop("JetDirect");
            gotoy(menutop+0); gotox(menuleft);
            cputs("IP Address:");
            gotoy(menutop+2); gotox(menuleft+4);
            cputs("_________________________");
            gotoy(menutop+2); gotox(menuleft+4);
            cputs(temp_host);
            paint_menu = 0;
        }

        gotox(menuleft+4+l);
        (*(char**)BASL)[menuleft+4+l] = 95;
        c = cgetc();
        if(c == 0x08) {
            if(l > 0) {
                // Remove flashing cursor
                gotox(menuleft+4+l);
                cputc('_');

                // Remove deleted character
                l--;
                gotox(menuleft+4+l);
                cputc('_');
                temp_host[l] = 0;
            }
        } else if((c == 0x0A) || (c == 0x0D)) {
            strcpy(jd_host, temp_host);
            return;
        } else if(c == 0x1B) {
            if(confirm("Go back without saving?")) {
                return;
            }
            paint_menu = 1;
        } else if(((c >= 'A') && (c <= 'Z')) || ((c >= 'a') && (c <= 'z')) || ((c >= '0') && (c <= '9')) || (c == '.')) {
            if(l < 24) {
                gotox(menuleft+4+l);
                cputc(c);
                temp_host[l] = c;
                l++;
                temp_host[l] = 0;
            }
        }
    }
}
    
void jdport_edit(void) {
    char temp_port[8];
    int c, l;
    int paint_menu = 1;
    
    sprintf(temp_port, "%i", jd_port);
    l = strlen(temp_port);

    for(;;) {
        if(paint_menu) {
            paint_backdrop("JetDirect");
            gotoy(menutop+0); gotox(menuleft);
            cputs("TCP Port:");
            gotoy(menutop+2); gotox(menuleft+4);
            cputs("______");
            gotoy(menutop+2); gotox(menuleft+4);
            cputs(temp_port);
            paint_menu = 0;
        }

        gotox(menuleft+4+l);
        (*(char**)BASL)[menuleft+4+l] = 95;
        c = cgetc();
        if(c == 0x08) {
            if(l > 0) {
                // Remove flashing cursor
                gotox(menuleft+4+l);
                cputc('_');

                // Remove deleted character
                l--;
                gotox(menuleft+4+l);
                cputc('_');
                temp_port[l] = 0;
            }
        } else if((c == 0x0A) || (c == 0x0D)) {
            jd_port = strtoul(temp_port, NULL, 0);
            return;
        } else if(c == 0x1B) {
            gotox(menuleft+4+l);
            cputc('_');
            if(confirm("Go back without saving?")) {
                return;
            }
            paint_menu = 1;
        } else if((c >= '0') && (c <= '9')) {
            if(l < 5) {
                gotox(menuleft+4+l);
                cputc(c);
                temp_port[l] = c;
                l++;
                temp_port[l] = 0;
            }
        }
    }
}

void ssid_edit(void) {
    char temp_ssid[32];
    int c, l;
    int paint_menu = 1;
    
    strcpy(temp_ssid, wifi_ssid);
    l = strlen(temp_ssid);

    for(;;) {
        if(paint_menu) {
            paint_backdrop("WiFi");
            gotoy(menutop+0); gotox(menuleft);
            cputs("Network Name (SSID):");
            gotoy(menutop+2); gotox(menuleft+4);
            cputs("_________________________");
            gotoy(menutop+2); gotox(menuleft+4);
            cputs(temp_ssid);
            paint_menu = 0;
        }

        gotox(menuleft+4+l);
        (*(char**)BASL)[menuleft+4+l] = 95;
        c = cgetc();
        if(c == 0x08) {
            if(l > 0) {
                // Remove flashing cursor
                gotox(menuleft+4+l);
                cputc('_');

                // Remove deleted character
                l--;
                gotox(menuleft+4+l);
                cputc('_');
                temp_ssid[l] = 0;
            }
        } else if((c == 0x0A) || (c == 0x0D)) {
            strcpy(wifi_ssid, temp_ssid);
            return;
        } else if(c == 0x1B) {
            gotox(menuleft+4+l);
            cputc('_');
            if(confirm("Go back without saving?")) {
                return;
            }
            paint_menu = 1;
        } else if(c >= 32) {
            if(l < 24) {
                gotox(menuleft+4+l);
                cputc(c);
                temp_ssid[l] = c;
                l++;
                temp_ssid[l] = 0;
            }
        }
    }
}

void psk_edit(void) {
    char temp_psk[32];
    int c, l;
    int paint_menu = 1;
    
    strcpy(temp_psk, wifi_psk);
    l = strlen(temp_psk);

    for(;;) {
        if(paint_menu) {
            paint_backdrop("WiFi");
            gotoy(menutop+0); gotox(menuleft);
            cputs("Passphrase (PSK):");
            gotoy(menutop+1); gotox(menuleft+4);
            cputs("_________________________");
            gotoy(menutop+1); gotox(menuleft+4);
            cputs(temp_psk);
            paint_menu = 0;
        }

        gotox(menuleft+4+l);
        (*(char**)BASL)[menuleft+4+l] = 95;
        c = cgetc();
        if(c == 0x08) {
            if(l > 0) {
                // Remove flashing cursor
                gotox(menuleft+4+l);
                cputc('_');

                // Remove deleted character
                l--;
                gotox(menuleft+4+l);
                cputc('_');
                temp_psk[l] = 0;
            }
        } else if((c == 0x0A) || (c == 0x0D)) {
            strcpy(wifi_psk, temp_psk);
            return;
        } else if(c == 0x1B) {
            gotox(menuleft+4+l);
            cputc('_');
            if(confirm("Go back without saving?")) {
                return;
            }
            paint_menu = 1;
        } else if(c >= 32) {
            if(l < 24) {
                gotox(menuleft+4+l);
                cputc(c);
                temp_psk[l] = c;
                l++;
                temp_psk[l] = 0;
            }
        }
    }
}

void address_edit(void) {
    char temp_address[32];
    int c, l;
    int paint_menu = 1;
    
    strcpy(temp_address, wifi_address);
    l = strlen(temp_address);

    for(;;) {
        if(paint_menu) {
            paint_backdrop("WiFi");
            gotoy(menutop+0); gotox(menuleft);
            cputs("Enter IP Address or \"DHCP\":");
            gotoy(menutop+2); gotox(menuleft+4);
            cputs("________________");
            gotoy(menutop+2); gotox(menuleft+4);
            cputs(temp_address);
            paint_menu = 0;
        }

        gotox(menuleft+4+l);
        (*(char**)BASL)[menuleft+4+l] = 95;
        c = cgetc();
        if(c == 0x08) {
            if(l > 0) {
                // Remove flashing cursor
                gotox(menuleft+4+l);
                cputc('_');

                // Remove deleted character
                l--;
                gotox(menuleft+4+l);
                cputc('_');
                temp_address[l] = 0;
            }
        } else if((c == 0x0A) || (c == 0x0D)) {
            strcpy(wifi_address, temp_address);
            return;
        } else if(c == 0x1B) {
            gotox(menuleft+4+l);
            cputc('_');
            if(confirm("Go back without saving?")) {
                return;
            }
            paint_menu = 1;
        } else if(c >= 32) {
            if(l < 15) {
                gotox(menuleft+4+l);
                cputc(c);
                temp_address[l] = c;
                l++;
                temp_address[l] = 0;
            }
        }
    }
}


void card_menu(void) {
    int paint_menu = 1;

    for(;;) {
        if(paint_menu) {
            paint_backdrop("Card Options");

            gotoy(menutop+0); gotox(menuleft);
            cputs("1. Mode: ");
            switch(v2mode) {
            case MODE_VGACARD:
                cputs("VGA Video");
                break;
            case MODE_APPLICARD:
                cputs("Applicard");
                break;
            case MODE_FS:
                cputs("Filesystem");
                break;
            case MODE_MIDI:
                cputs("MIDI");
                break;
            case MODE_SNESMAX:
                cputs("SNESMAX");
                break;
            case MODE_ETHERNET:
                cputs("Ethernet");
                break;
            case MODE_SERIAL:
                cputs("Serial");
                gotoy(menutop+2); gotox(menuleft);
                printf("2. Baudrate: %i", baud);
                gotoy(menutop+4); gotox(menuleft);
                cputs("3. Device Mux: ");
                switch(serialmux) {
                case SERIAL_PRINTER:
                    cputs("Printer");
                    gotoy(menutop+6); gotox(menuleft);
                    printf("4. Host: %s", jd_host);
                    gotoy(menutop+8); gotox(menuleft);
                    printf("5. Port: %i", jd_port);
                    break;
                case SERIAL_USB:
                    cputs("USB");
                    gotoy(menutop+10); gotox(menuleft);
                    cputs("6. USB Mode: ");
                    switch(usbmux) {
                    case USB_HOST_CDC:
                        cputs("CDC Host");
                        break;
                    case USB_GUEST_CDC:
                        cputs("CDC Guest");
                        break;
                    }
                    break;
                    break;
                case SERIAL_LOOP:
                    cputs("Loopback");
                    break;
                case SERIAL_WIFI:
                    cputs("WiFi Modem");
                    break;
                }
                break;
            case MODE_PARALLEL:
                cputs("Parallel");
                gotoy(menutop+4); gotox(menuleft);
                cputs("3. Device Mux: ");
                switch(serialmux) {
                case SERIAL_PRINTER:
                    cputs("Printer");
                    gotoy(menutop+6); gotox(menuleft);
                    printf("4. Host: %s", jd_host);
                    gotoy(menutop+8); gotox(menuleft);
                    printf("5. Port: %i", jd_port);
                    break;
                case SERIAL_USB:
                    cputs("USB");
                    gotoy(menutop+10); gotox(menuleft);
                    cputs("6. USB Mode: ");
                    switch(usbmux) {
                    case USB_HOST_CDC:
                        cputs("CDC Host");
                        break;
                    case USB_GUEST_CDC:
                        cputs("CDC Guest");
                        break;
                    }
                    break;
                default:
                    cputs(">NUL:");
                }
                break;
            }

            gotoy(menutop+17); gotox(menuleft);
            cputs("9. Back to Main Menu");
            paint_menu = 0;
        }
        switch(cgetc()) {
        case '1':
            mode_menu();
            paint_menu = 1;
            break;
        case '2':
            if(v2mode == MODE_SERIAL) {
                baud_menu();
                paint_menu = 1;
            }
            break;
        case '3':
            if((v2mode == MODE_SERIAL) || (v2mode == MODE_PARALLEL)) {
                mux_menu();
                paint_menu = 1;
            }
            break;
        case '4':
            if(((v2mode == MODE_SERIAL) || (v2mode == MODE_PARALLEL)) && (serialmux == SERIAL_PRINTER)) {
                jdhost_edit();
                paint_menu = 1;
            }
            break;
        case '5':
            if(((v2mode == MODE_SERIAL) || (v2mode == MODE_PARALLEL)) && (serialmux == SERIAL_PRINTER)) {
                jdport_edit();
                paint_menu = 1;
            }
            break;
        case '6':
            if(((v2mode == MODE_SERIAL) || (v2mode == MODE_PARALLEL)) && (serialmux == SERIAL_USB)) {
                usbmux = (usbmux == USB_HOST_CDC) ? USB_GUEST_CDC : USB_HOST_CDC;
                paint_menu = 1;
            }
            break;
        case 0x1B:
        case '9':
            return;
        }
    }
}

void wifi_menu(void) {
    int paint_menu = 1;
    int c;
    for(;;) {
        if(paint_menu) {
            paint_backdrop("WiFi Options");

            gotoy(menutop+0); gotox(menuleft);
            cputs("1. WIFI Mode:");
            gotoy(menutop+2); gotox(8);
            switch(wifimode) {
            case WIFI_AP:
                cputs("Access Point");
                break;
            case WIFI_CLIENT:
                cputs("Client");
                break;
            default:
                cputs("Unknown");
            }
            gotoy(menutop+4); gotox(menuleft);
            cputs("2. Network Name:");
            gotoy(menutop+6); gotox(8);
            cputs(wifi_ssid);
            gotoy(menutop+8); gotox(menuleft);
            cputs("3. Pre-Shared Key:");
            gotoy(menutop+10); gotox(8);
            for(c = 0; c < strlen(wifi_psk); c++) {
                cputc('*');
            }
            gotoy(menutop+12); gotox(menuleft);
            cputs("4. IP Address:");
            gotoy(menutop+14); gotox(8);
            if(wifimode == WIFI_AP) {
                cputs("10.65.50.1");
            } else {
                cputs(wifi_address);
            }

            gotoy(menutop+17); gotox(menuleft);
            puts("9. Back to Main Menu");
            paint_menu = 0;
        }

        switch(cgetc()) {
        case '1':
            wifimode = (wifimode == WIFI_AP) ? WIFI_CLIENT : WIFI_AP;
            paint_menu = 1;
            break;
        case '2':
            ssid_edit();
            paint_menu = 1;
            break;
        case '3':
            psk_edit();
            paint_menu = 1;
            break;
        case '4':
            if(wifimode == WIFI_AP) {
                message("Address is fixed in this mode.");
                paint_menu = 1;
            } else {
                address_edit();
                paint_menu = 1;
            }
            break;
        case 0x1B:
        case '9':
            return;
        }
    }
}

void machine_menu(void) {
    int paint_menu = 1;
    for(;;) {
        if(paint_menu) {
            paint_backdrop("Machine Options");

            gotoy(menutop+0); gotox(menuleft);
            cputs("1. Host Type:");
            gotoy(menutop+2); gotox(8);
            switch(machine) {
            case MACHINE_AUTO:
                cputs("Auto Detect");
                break;
            case MACHINE_APPLE_IIGS:
                cputs("Apple IIgs");
                break;
            case MACHINE_APPLE_IIE:
                cputs("Apple IIe");
                break;
            default:
                cputs("Apple II / II+");
            }

            gotoy(menutop+17); gotox(menuleft);
            puts("9. Back to Main Menu");
            paint_menu = 0;
        }

        switch(cgetc()) {
        case '1':
            machine = (machine + 1);
            if(machine >= MACHINE_MAXVALUE)
                machine = 0;
            paint_menu = 1;
            break;
        case 0x1B:
        case '9':
            return;
        }
    }
}

int prompt_slot(void) {
    int c;
    paint_backdrop("Config Utility");

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

    if(!prompt_slot()) {
        return;
    }

    config_cmdbuf = (uint8_t *)(0xC0F0 | (cardslot << 8));
    config_errbuf = (uint8_t *)(0xC0F8 | (cardslot << 8));

    paint_backdrop("Please Wait");
    gotoy(11); gotox(9);
    cputs("Reading configuration,");
    gotoy(12); gotox(8);
    cputs("your screen may flicker.");

    read_config();

    for(;;) {
        if(paint_menu) {
            paint_backdrop("Main Menu");

            gotoy(menutop+0); gotox(menuleft);
            cputs("1. Card options");
            gotoy(menutop+2); gotox(menuleft);
            cputs("2. Machine options");

#if 0
            gotoy(menutop+4); gotox(menuleft);
            cputs("3. WIFI options");
            gotoy(menutop+9); gotox(menuleft);
            cputs("5. Backup settings");
            gotoy(menutop+11); gotox(menuleft);
            cputs("6. Restore settings");
#endif
            gotoy(menutop+13); gotox(menuleft);
            cputs("7. Format card storage");
            gotoy(menutop+15); gotox(menuleft);
            cputs("8. Quit without saving");
            gotoy(menutop+17); gotox(menuleft);
            cputs("9. Save & Exit");
            paint_menu = 0;
        }

        switch(cgetc()) {
        case '1':
            card_menu();
            paint_menu = 1;
            break;
        case '2':
            machine_menu();
            paint_menu = 1;
            break;
#if 0
        case '3':
            wifi_menu();
            paint_menu = 1;
            break;
        case '5':
        case '6':
            message("Function Not Implemented");
            paint_menu = 1;
            break;
#endif
        case '7':
            if(confirm("Format Card?")) {
                format_card();
                clrscr();
                rebootafterexit();
                return;
            }
            paint_menu = 1;
            break;
        case '8':
            if(confirm("Quit without saving?")) {
                clrscr();
                return;
            }
            paint_menu = 1;
            break;
        case '9':
            if(confirm("Save and exit?")) {
                write_config();
                clrscr();
                rebootafterexit();
                return;
            }
            paint_menu = 1;
            break;
        }
    }
}
