#include <string.h>
#include <pico/stdlib.h>
#include <pico/multicore.h>
#include "common/config.h"
#include "common/buffers.h"
#include "fs/businterface.h"
#include "fs/fs.h"
#include "pico_hal.h"

typedef struct {
    int handle;
    bool valid;
} fs_file_t;

#define FS_MAXFILE 8
fs_file_t fs_file[FS_MAXFILE];

void fs_open(volatile uint8_t *fsregs) {
    int file;
    for(int i = 0; i < FS_MAXFILE; i++) {
        if(!fs_file[i].valid) {
            int flags = 0;
            if(fsregs[FS_FLAGS] & FS_O_RD)
                flags |= LFS_O_RDONLY;
            if(fsregs[FS_FLAGS] & FS_O_WR)
                flags |= LFS_O_WRONLY;
            if(fsregs[FS_FLAGS] & FS_O_CREATE)
                flags |= LFS_O_CREAT;
            if(fsregs[FS_FLAGS] & FS_O_APPEND)
                flags |= LFS_O_APPEND;
            if(fsregs[FS_FLAGS] & FS_O_EXISTING)
                flags |= LFS_O_EXCL;
            if(fsregs[FS_FLAGS] & FS_O_TRUNC)
                flags |= LFS_O_TRUNC;

            if((flags & LFS_O_RDWR) != 0) {
                // Ensure pathname is 128 bytes or less.
                apple_memory[0xC880] = 0x00;
                file = pico_open((char*)(apple_memory + 0xC800), flags);
            } else {
                fsregs[FS_STATUS] = FS_ERR_INVAL;
                fsregs[FS_FILE] = FS_ERR_INVAL;
                return;
            }

            if(file < 0) {
                fsregs[FS_STATUS] = file;
                fsregs[FS_FILE] = file;
                return;
            } else {
                fs_file[i].handle = file;
                fs_file[i].valid = true;
                fsregs[FS_STATUS] = FS_ERR_OK;
                fsregs[FS_FILE] = i;
                return;
            }
        }
    }
    
    fsregs[FS_STATUS] = FS_ERR_NOMEM;
}

void fs_close(volatile uint8_t *fsregs) {
    int i = fsregs[FS_FILE];
    if((i >= FS_MAXFILE) || (!fs_file[i].valid) || (fs_file[i].handle < 0)) {
        fsregs[FS_STATUS] = FS_ERR_INVAL;
        return;
    }
    
    pico_close(fs_file[i].handle);
    fs_file[i].handle = -1;
    fs_file[i].valid = false;
}

void fs_read(volatile uint8_t *fsregs) {
    int i = fsregs[FS_FILE];
    uint16_t size = (((uint16_t)fsregs[FS_OFFMSB]) << 8) | ((uint16_t)fsregs[FS_SIZELSB]);

    if((i >= FS_MAXFILE) || (!fs_file[i].valid) || (fs_file[i].handle < 0) || (size > 512)) {
        fsregs[FS_STATUS] = FS_ERR_INVAL;
        return;
    }
    if(size == 0) {
        fsregs[FS_STATUS] = FS_ERR_OK;
    }

    int rv = pico_read(fs_file[i].handle, (uint8_t*)(apple_memory+0xC800), size);
    if(rv < 0) {
        fsregs[FS_STATUS] = rv;
    } else {
        fsregs[FS_SIZELSB] = rv & 0xFF;
        fsregs[FS_SIZEMSB] = (rv >> 8);
        fsregs[FS_STATUS] = FS_ERR_OK;
    }
}

void fs_write(volatile uint8_t *fsregs) {
    int i = fsregs[FS_FILE];
    uint16_t size = (((uint16_t)fsregs[FS_OFFMSB]) << 8) | ((uint16_t)fsregs[FS_SIZELSB]);

    if((i >= FS_MAXFILE) || (!fs_file[i].valid) || (fs_file[i].handle < 0) || (size > 512)) {
        fsregs[FS_STATUS] = FS_ERR_INVAL;
        return;
    }
    if(size == 0) {
        fsregs[FS_STATUS] = FS_ERR_OK;
    }

    int rv = pico_write(fs_file[i].handle, (uint8_t*)(apple_memory+0xC800), size);
    if(rv < 0) {
        fsregs[FS_STATUS] = rv;
    } else {
        fsregs[FS_SIZELSB] = rv & 0xFF;
        fsregs[FS_SIZEMSB] = (rv >> 8);
        fsregs[FS_STATUS] = FS_ERR_OK;
    }
}

void fs_seek(volatile uint8_t *fsregs) {
    int i = fsregs[FS_FILE];
    int16_t off = (int16_t)((((uint16_t)fsregs[FS_OFFMSB]) << 8) | ((uint16_t)fsregs[FS_OFFLSB]));
    int whence = fsregs[FS_WHENCE];

    if((i >= FS_MAXFILE) || (!fs_file[i].valid) || (fs_file[i].handle < 0)) {
        fsregs[FS_STATUS] = FS_ERR_INVAL;
        return;
    }

    int rv = pico_lseek(fs_file[i].handle, off, whence);
    if(rv < 0) {
        fsregs[FS_STATUS] = rv;
    } else {
        fsregs[FS_OFFLSB] = rv & 0xFF;
        fsregs[FS_OFFMSB] = (rv >> 8);
        fsregs[FS_STATUS] = FS_ERR_OK;
    }
}

void fs_tell(volatile uint8_t *fsregs) {
    int i = fsregs[FS_FILE];

    if((i >= FS_MAXFILE) || (!fs_file[i].valid) || (fs_file[i].handle < 0)) {
        fsregs[FS_STATUS] = FS_ERR_INVAL;
        return;
    }

    int rv = pico_tell(fs_file[i].handle);
    if(rv < 0) {
        fsregs[FS_STATUS] = rv;
    } else {
        fsregs[FS_OFFLSB] = rv & 0xFF;
        fsregs[FS_OFFMSB] = (rv >> 8);
        fsregs[FS_STATUS] = FS_ERR_OK;
    }
}

void prodos_command(volatile uint8_t *fsregs) {
}

void fs_handler(uint8_t slot) {
    volatile uint8_t *fsregs = apple_memory + 0xC080 + (slot << 4);
    if(fsregs[FS_EXECUTE] != 0x00) {
        fsregs[FS_STATUS] = FS_ERR_INVAL;
        return;
    }
    
    fsregs[FS_BUSY] = 0xff;

    switch(fsregs[FS_COMMAND]) {
    case FS_OPEN:
        fs_open(fsregs);
        break;
    case FS_CLOSE:
        fs_close(fsregs);
        break;
    case FS_READ:
        fs_read(fsregs);
        break;
    case FS_WRITE:
        fs_write(fsregs);
        break;
    case FS_SEEK:
        fs_seek(fsregs);
        break;
    case FS_TELL:
        fs_tell(fsregs);
        break;
    case PRODOS_COMMAND:
        prodos_command(fsregs);
        break;
    }

    fsregs[FS_BUSY] = 0x00;
}

void fsmain() {
    int i;

    memset((uint8_t*)(apple_memory+0xC000), 0x00, 0x1000);

    for(i = 0; i < FS_MAXFILE; i++) {
        fs_file[i].handle = -1;
        fs_file[i].valid = false;
    }

    // Attempt to open boot disk image
    fs_file[0].handle = pico_open("boot.dsk", LFS_O_RDONLY);
    fs_file[0].valid = (fs_file[0].handle >= 0);

    strcpy((uint8_t*)(apple_memory+0xC1E0), "FSREADY.");
    strcpy((uint8_t*)(apple_memory+0xC2E0), "FSREADY.");
    strcpy((uint8_t*)(apple_memory+0xC3E0), "FSREADY.");
    strcpy((uint8_t*)(apple_memory+0xC4E0), "FSREADY.");
    strcpy((uint8_t*)(apple_memory+0xC5E0), "FSREADY.");
    strcpy((uint8_t*)(apple_memory+0xC6E0), "FSREADY.");
    strcpy((uint8_t*)(apple_memory+0xC7E0), "FSREADY.");

    while(v2mode == MODE_FS) {
        sleep_ms(50);
    }

    // Close any open handles
    for(i = 0; i < FS_MAXFILE; i++) {
        if(fs_file[i].valid) {
            pico_close(fs_file[i].handle);
            fs_file[i].handle = -1;
            fs_file[i].valid = false;
        }
    }
}

