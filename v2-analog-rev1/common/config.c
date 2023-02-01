#include "common/config.h"
#include "common/buffers.h"
#include "common/flash.h"
#include "pico_hal.h"

volatile v2mode_t cfg_mode = MODE_UNKNOWN;
volatile v2mode_t current_mode = MODE_UNKNOWN;

volatile usbmux_t usbmux;
volatile serialmux_t serialmux;
volatile wifimode_t wifimode;

volatile compat_t cfg_machine = MACHINE_AUTO;
volatile compat_t current_machine = MACHINE_AUTO;

volatile uint8_t wifi_ssid[32];
volatile uint8_t wifi_psk[32];

extern volatile bool userfont;

char ERR_READY[8]  = { 'R', 'E', 'A', 'D', 'Y',   0,   0,   0 };
char ERR_EOF[8]    = { 'E', 'O', 'F',   0,   0,   0,   0,   0 };
char ERR_NOFILE[8] = { 'E', 'O', 'F',   0,   0,   0,   0,   0 };

void parse_config(uint8_t *buffer) {
    if(!memcmp("C=", buffer, 2)) {
        if(!strcmp("DIAG", buffer+2)) {
            cfg_mode = MODE_DIAG;
        } else if(!strcmp("FS", buffer+2)) {
            cfg_mode = MODE_FS;
        } else if(!strcmp("VGA", buffer+2)) {
            cfg_mode = MODE_VGACARD;
        } else if(!strcmp("Z80", buffer+2)) {
            cfg_mode = MODE_APPLICARD;
        } else if(!strcmp("SER", buffer+2)) {
            cfg_mode = MODE_SERIAL;
        } else if(!strcmp("PAR", buffer+2)) {
            cfg_mode = MODE_PARALLEL;
        } else if(!strcmp("SNES", buffer+2)) {
            cfg_mode = MODE_SNESMAX;
        }
    } else if(!memcmp("H=", buffer, 2)) {
        if(!strcmp("AUTO", buffer+2)) {
            cfg_machine = MACHINE_AUTO;
        } else if(!strcmp("II", buffer+2)) {
            cfg_machine = MACHINE_II;
            soft_switches &= ~(SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS);
        } else if(!strcmp("IIE", buffer+2)) {
            cfg_machine = MACHINE_IIE;
            soft_switches &= ~SOFTSW_IIGS_REGS;
            soft_switches |= SOFTSW_IIE_REGS;
        } else if(!strcmp("IIGS", buffer+2)) {
            cfg_machine = MACHINE_IIGS;
            soft_switches |= SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS;
        } else if(!strcmp("PRAVETZ", buffer+2)) {
            cfg_machine = MACHINE_PRAVETZ;
            soft_switches &= ~(SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS);
        }
    } else if(!memcmp("S=", buffer, 2)) {
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
    } else if(!memcmp("WN=", buffer, 3)) {
        strncpy((char*)wifi_ssid, buffer+5, 24);
    } else if(!memcmp("WP=", buffer, 3)) {
        strncpy((char*)wifi_psk, buffer+5, 24);
    }
}

void default_config() {
    cfg_mode = MODE_VGACARD;
    serialmux = SERIAL_LOOP;
    usbmux = USB_GUEST_CDC;
    wifimode = WIFI_AP;
    strcpy((char*)wifi_ssid, "V2RetroNet");
    strcpy((char*)wifi_psk, "Analog");
    cfg_machine = MACHINE_AUTO;
}

int make_config(uint8_t *buf, uint16_t len) {
    uint16_t ptr = 0;
    
    memset(buf, 0, len);

    switch(cfg_mode) {
    default:
    case MODE_VGACARD:
        strcpy(buf+ptr, "C=VGA");
        ptr += 32;
        break;
    case MODE_APPLICARD:
        strcpy(buf+ptr, "C=Z80");
        ptr += 32;
        break;
    case MODE_SERIAL:
        strcpy(buf+ptr, "C=SER");
        ptr += 32;
        break;
    case MODE_PARALLEL:
        strcpy(buf+ptr, "C=PAR");
        ptr += 32;
        break;
    case MODE_SNESMAX:
        strcpy(buf+ptr, "C=SNES");
        ptr += 32;
        break;
    case MODE_ETHERNET:
        strcpy(buf+ptr, "C=NET");
        ptr += 32;
        break;
    case MODE_FS:
        strcpy(buf+ptr, "C=FS");
        ptr += 32;
        break;
    }
    switch(cfg_machine) {
    default:
    case MACHINE_AUTO:
        strcpy(buf+ptr, "H=AUTO");
        ptr += 32;
        break;
    case MACHINE_II:
        strcpy(buf+ptr, "H=II");
        ptr += 32;
        break;
    case MACHINE_IIE:
        strcpy(buf+ptr, "H=IIE");
        ptr += 32;
        break;
    case MACHINE_IIGS:
        strcpy(buf+ptr, "H=IIGS");
        ptr += 32;
        break;
    }
    switch(serialmux) {
    default:
    case SERIAL_LOOP:
        strcpy(buf+ptr, "M=LOOP");
        ptr += 32;
        break;
    case SERIAL_USB:
        strcpy(buf+ptr, "M=USB");
        ptr += 32;
        break;
    case SERIAL_WIFI:
        strcpy(buf+ptr, "M=WIFI");
        ptr += 32;
        break;
    case SERIAL_PRINTER:
        strcpy(buf+ptr, "M=PRN");
        ptr += 32;
        break;
    }
    switch(usbmux) {
    case USB_GUEST_CDC:
        strcpy(buf+ptr, "U=CG");
        ptr += 32;
        break;
    case USB_HOST_CDC:
        strcpy(buf+ptr, "U=CH");
        ptr += 32;
        break;
    }

    switch(wifimode) {
    case WIFI_CLIENT:
        strcpy(buf+ptr, "W=C");
        ptr += 32;
        break;
    case WIFI_AP:
        strcpy(buf+ptr, "W=A");
        ptr += 32;
        break;
    }

    strcpy(buf+ptr, "WS=");
    strncpy(buf+ptr+2, (char*)wifi_ssid, 24);
    ptr += 32;

    strcpy(buf+ptr, "WP=");
    strncpy(buf+ptr+2, (char*)wifi_psk, 24);
    ptr += 32;
    
    return ptr;
}

void write_config() {
    uint8_t config_temp[1024];
    int config_len;
    config_len = make_config(config_temp, sizeof(config_temp));
    
    int file = pico_open("config", LFS_O_WRONLY | LFS_O_CREAT);
    if(file < 0)
        return;

    pico_write(file, config_temp, config_len);
    pico_close(file);
}

void read_config() {
    uint8_t config_temp[1024];
    int file = pico_open("config", LFS_O_RDONLY);
    int config_len = 0;

    if(file < 0)
        return;

    memset(config_temp, 0, sizeof(config_temp));
    config_len = pico_read(file, config_temp, sizeof(config_temp));
    pico_close(file);

    if(config_len > 0) {
        for(int ptr = 0; ptr < config_len; ptr+=32) {
            parse_config(config_temp+ptr);
        }
    }
}

extern uint8_t character_rom[2048];

void upload_config() {
    int file = pico_open("config", LFS_O_RDONLY);

    memset((uint8_t*)(apple_memory + 0xC800), 0, 1024);
    if(file < 0) {
        make_config((uint8_t*)(apple_memory + 0xC800), 1024);
    } else {
        pico_read(file, (uint8_t*)(apple_memory + 0xC800), 1024);
        pico_close(file);
    }
    
    memcpy(config_errbuf, ERR_READY, sizeof(ERR_READY));
}

void download_config(uint16_t ptr) {
    int file = pico_open("config", LFS_O_WRONLY | LFS_O_CREAT);

    if(file < 0) {
        memcpy(config_errbuf, ERR_NOFILE, sizeof(ERR_NOFILE));
        return;
    }

    pico_write(file, (uint8_t*)(apple_memory + ptr), 1024);
    pico_close(file);

    memcpy(config_errbuf, ERR_READY, sizeof(ERR_READY));
}


void upload_font(uint8_t page) {
    int file;
    if(page > 1) {
        memcpy(config_errbuf, ERR_EOF, sizeof(ERR_EOF));
        return;
    }

    file = pico_open("font", LFS_O_RDONLY);
    if(file < 0) {
        memcpy((uint8_t*)(apple_memory + 0xC800), (character_rom + (1024*page)), 1024);
    } else {
        pico_lseek(file, 1024 * page, LFS_SEEK_SET);
        pico_read(file, (uint8_t*)(apple_memory + 0xC800), 1024);
        pico_close(file);
    }

    memcpy(config_errbuf, ERR_READY, sizeof(ERR_READY));
}

void download_font(uint16_t ptr) {
    int file = pico_open("font", LFS_O_WRONLY | LFS_O_CREAT);

    memcpy(character_rom, (uint8_t*)(apple_memory + ptr), 2048);

    if(file < 0) {
        memcpy(config_errbuf, ERR_NOFILE, sizeof(ERR_NOFILE));
        return;
    }

    pico_write(file, (uint8_t*)(apple_memory + ptr), 2048);
    pico_close(file);

    memcpy(config_errbuf, ERR_READY, sizeof(ERR_READY));
}

void config_format() {
    pico_unmount();

    if(pico_mount(1) != LFS_ERR_OK) {
        memcpy(config_errbuf, ERR_NOFILE, sizeof(ERR_NOFILE));
    } else {
        memcpy(config_errbuf, ERR_READY, sizeof(ERR_READY));
    }
}

void config_handler() {
    if(config_cmdbuf[7] != 0) return;

    config_cmdbuf[7] = 0xff;

    if(!memcmp("H=", (uint8_t*)config_cmdbuf, 2)) {
        if(!strcmp("II", config_cmdbuf+2)) {
            current_machine = MACHINE_II;
            soft_switches &= ~(SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS);
        } else if(!strcmp("IIE", config_cmdbuf+2)) {
            current_machine = MACHINE_IIE;
            soft_switches &= ~SOFTSW_IIGS_REGS;
            soft_switches |= SOFTSW_IIE_REGS;
        } else if(!strcmp("IIGS", config_cmdbuf+2)) {
            current_machine = MACHINE_IIGS;
            soft_switches |= SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS;
#if 0
        } else if(!strcmp("B108", config_cmdbuf+2)) {
            current_machine = MACHINE_BASIS;
            soft_switches &= ~(SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS);
        } else if(!strcmp("P", config_cmdbuf+2)) {
            current_machine = MACHINE_PRAVETZ;
            soft_switches &= ~(SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS);
        } else if(!strcmp("A7", config_cmdbuf+2)) {
            current_machine = MACHINE_AGAT7;
            soft_switches &= ~(SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS);
        } else if(!strcmp("A9", config_cmdbuf+2)) {
            current_machine = MACHINE_AGAT9;
            soft_switches &= ~(SOFTSW_IIE_REGS | SOFTSW_IIGS_REGS);
#endif
        }
    } else if(!memcmp("UF", (uint8_t*)config_cmdbuf, 2)) {
        upload_font(config_cmdbuf[2]);
    } else if(!memcmp("DF", (uint8_t*)config_cmdbuf, 2)) {
        download_font((config_cmdbuf[2] << 0) | (config_cmdbuf[3] << 8));
        flash_reboot();
    } else if(!memcmp("UC", (uint8_t*)config_cmdbuf, 2)) {
        upload_config();
    } else if(!memcmp("DC", (uint8_t*)config_cmdbuf, 2)) {
        download_config((config_cmdbuf[2] << 0) | (config_cmdbuf[3] << 8));
        flash_reboot();
    } else if(!memcmp("FORMAT", (uint8_t*)config_cmdbuf, 6)) {
        config_format();
        flash_reboot();
    }
    
    memset(config_cmdbuf, 0x00, 7);
}
