#include "common/config.h"
#include "common/buffers.h"
#include "pico_hal.h"

volatile uint8_t config_memory[32];

v2mode_t v2mode;
usbmux_t usbmux;
serialmux_t serialmux;
wifimode_t wifimode;
compat_t machine;
uint8_t wifi_ssid[32];
uint8_t wifi_psk[32];

extern bool userfont;

void parse_config(uint8_t *buffer) {
    if(!memcmp("MODE=", buffer, 5)) {
        if(!strcmp("DIAG", buffer+5)) {
            v2mode = MODE_DIAG;
        } else if(!strcmp("FS", buffer+5)) {
            v2mode = MODE_FS;
        } else if(!strcmp("VGA", buffer+5)) {
            v2mode = MODE_VGACARD;
        } else if(!strcmp("Z80", buffer+5)) {
            v2mode = MODE_APPLICARD;
        } else if(!strcmp("SERIAL", buffer+5)) {
            v2mode = MODE_SERIAL;
        } else if(!strcmp("PARALLEL", buffer+5)) {
            v2mode = MODE_PARALLEL;
        } else if(!strcmp("SNESMAX", buffer+5)) {
            v2mode = MODE_SNESMAX;
        }
    } else if(!memcmp("MACHINE=", buffer, 8)) {
        if(!strcmp("II", buffer+8)) {
            machine = APPLE_II;
            soft_switches &= ~(SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS);
        } else if(!strcmp("IIE", buffer+8)) {
            machine = APPLE_IIE;
            soft_switches &= ~SOFTSW_IIGS_REGS;
            soft_switches |= SOFTSW_IIE_REGS;
        } else if(!strcmp("IIGS", buffer+8)) {
            machine = APPLE_IIGS;
            soft_switches |= SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS;
        }
    } else if(!memcmp("MUX=", buffer, 4)) {
        if(!strcmp("USB", buffer+4)) {
            serialmux = SERIAL_USB;
        } else if(!strcmp("MODEM", buffer+4)) {
            serialmux = SERIAL_WIFI;
        } else if(!strcmp("PRINTER", buffer+4)) {
            serialmux = SERIAL_PRINTER;
        } else if(!strcmp("LOOP", buffer+4)) {
            serialmux = SERIAL_LOOP;
        }
    } else if(!memcmp("USB=", buffer, 4)) {
        if(!strcmp("CDC_GUEST", buffer+4)) {
            usbmux = USB_GUEST_CDC;
        } else if(!strcmp("CDC_HOST", buffer+4)) {
            usbmux = USB_HOST_CDC;
        }
    } else if(!memcmp("WIFI=", buffer, 5)) {
        if(!strcmp("CLIENT", buffer+5)) {
            wifimode = WIFI_CLIENT;
        } else if(!strcmp("AP", buffer+5)) {
            wifimode = WIFI_AP;
        }
    } else if(!memcmp("SSID=", buffer, 5)) {
        strncpy(wifi_ssid, buffer+5, 25);
    } else if(!memcmp("PSK=", buffer, 4)) {
        strncpy(wifi_psk, buffer+5, 26);
    }
}

void default_config() {
    v2mode = MODE_VGACARD;
    serialmux = SERIAL_LOOP;
    usbmux = USB_GUEST_CDC;
    wifimode = WIFI_AP;
    strcpy(wifi_ssid, "V2RetroNet");
    strcpy(wifi_psk, "Analog");
    machine = COMPAT_AUTO;
    soft_switches = 0;
}

void write_config() {
    uint8_t config_temp[32];
    
    if(v2mode == MODE_DIAG) return;
    if(v2mode == MODE_REBOOT) return;
    
    int file = pico_open("config", LFS_O_WRONLY | LFS_O_CREAT);
    if(file < 0)
        return;
    
    switch(v2mode) {
    case MODE_FS:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MODE=FS");
        pico_write(file, config_temp, 32);
        break;
    case MODE_VGACARD:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MODE=VGA");
        pico_write(file, config_temp, 32);
        break;
    case MODE_APPLICARD:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MODE=Z80");
        pico_write(file, config_temp, 32);
        break;
    case MODE_SERIAL:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MODE=SERIAL");
        pico_write(file, config_temp, 32);
        break;
    case MODE_PARALLEL:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MODE=PARALLEL");
        pico_write(file, config_temp, 32);
        break;
    case MODE_SNESMAX:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MODE=SNESMAX");
        pico_write(file, config_temp, 32);
        break;
    case MODE_ETHERNET:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MODE=ETHERNET");
        pico_write(file, config_temp, 32);
        break;
    }
    switch(machine) {
    case APPLE_II:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MACHINE=II");
        pico_write(file, config_temp, 32);
        break;
    case APPLE_IIE:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MACHINE=IIE");
        pico_write(file, config_temp, 32);
        break;
    case APPLE_IIGS:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MACHINE=IIGS");
        pico_write(file, config_temp, 32);
        break;
    }
    switch(serialmux) {
    case SERIAL_USB:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MUX=USB");
        pico_write(file, config_temp, 32);
        break;
    case SERIAL_WIFI:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MUX=WIFI");
        pico_write(file, config_temp, 32);
        break;
    case SERIAL_PRINTER:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MUX=PRINTER");
        pico_write(file, config_temp, 32);
        break;
    case SERIAL_LOOP:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "MUX=LOOP");
        pico_write(file, config_temp, 32);
        break;
    }
    switch(usbmux) {
    case USB_GUEST_CDC:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "USB=CDC_GUEST");
        pico_write(file, config_temp, 32);
        break;
    case USB_HOST_CDC:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "USB=CDC_HOST");
        pico_write(file, config_temp, 32);
        break;
    }

    switch(wifimode) {
    case WIFI_CLIENT:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "WIFI=CLIENT");
        pico_write(file, config_temp, 32);
        break;
    case WIFI_AP:
        memset(config_temp, 0, sizeof(config_temp));
        strcpy(config_temp, "WIFI=AP");
        pico_write(file, config_temp, 32);
        break;
    }

    memset(config_temp, 0, sizeof(config_temp));
    strcpy(config_temp, "SSID=");
    strncat(config_temp, wifi_ssid, 25);
    pico_write(file, config_temp, 32);

    memset(config_temp, 0, sizeof(config_temp));
    strcpy(config_temp, "PSK=");
    strncat(config_temp, wifi_psk, 25);
    pico_write(file, config_temp, 32);

    pico_close(file);
}

void read_config() {
    uint8_t config_temp[32];
    int file = pico_open("config", LFS_O_RDONLY);
    int br = 0;

    if(file < 0)
        return;

    do {
        br = pico_read(file, config_temp, 32);
        if(br > 0) {
            parse_config(config_temp);
        }
    } while(br > 0);

    pico_close(file);
}

volatile uint8_t *videx_page_tmp = (private_memory+0xF000);
extern uint8_t character_rom[4096];

void config_handler() {
    if(config_memory[31] != 0) return;

    if(!strcmp("BANK=SAVE", (uint8_t*)config_memory)) {
        videx_page_tmp = videx_page;
    } else if(!strcmp("BANK=RESTORE", (uint8_t*)config_memory)) {
        videx_page = videx_page_tmp;
    } else if(!memcmp("BANK=FONT", (uint8_t*)config_memory, 9)) {
        videx_page = character_rom + (((config_memory[9] - '0') & 3) * 1024);
    } else if(!strcmp("WRITE_CONFIG", (uint8_t*)config_memory)) {
        write_config();
    } else if(!strcmp("REBOOT", (uint8_t*)config_memory)) {
        v2mode = MODE_REBOOT;
    } else parse_config((uint8_t*)config_memory);
}
