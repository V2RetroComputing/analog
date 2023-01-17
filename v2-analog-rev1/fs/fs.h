#pragma once

#define FS_COMMAND     0x0
#define FS_FILE        0x1
#define FS_FLAGS       0x1
#define FS_SIZELSB     0x2
#define FS_SIZEMSB     0x3
#define FS_OFFLSB      0x2
#define FS_OFFMSB      0x3
#define FS_WHENCE      0x4
#define FS_BUSY        0xD
#define FS_STATUS      0xE
#define FS_EXECUTE     0xF

#define FS_O_RD        1
#define FS_O_WR        2
#define FS_O_APPEND    4
#define FS_O_EXISTING  8
#define FS_O_CREATE    16
#define FS_O_TRUNC     32

#define FS_SEEK_SET    0
#define FS_SEEK_CUR    1
#define FS_SEEK_END    2

typedef enum {
    FS_OPEN  = 0x10,
    FS_CLOSE = 0x11,
    FS_READ  = 0x12,
    FS_WRITE = 0x13,
    FS_SEEK  = 0x14,
    FS_TELL  = 0x15,
    PRODOS_COMMAND = 0x20,
} fscommand_t;

typedef enum {
    FS_ERR_OK = 0,           // No error
    FS_ERR_IO = -1,          // Error during device operation
    FS_ERR_CORRUPT = -2,     // Corrupted
    FS_ERR_NOENT = -3,       // No directory entry
    FS_ERR_EXIST = -4,       // Entry already exists
    FS_ERR_NOTDIR = -5,      // Entry is not a dir
    FS_ERR_ISDIR = -5,       // Entry is a dir
    FS_ERR_NOTEMPTY = -7,    // Dir is not empty
    FS_ERR_BADF = -8,        // Bad file number
    FS_ERR_FBIG = -9,        // File too large
    FS_ERR_INVAL = -10,      // Invalid parameter
    FS_ERR_NOSPC = -11,      // No space left on device
    FS_ERR_NOMEM = -12,      // No more memory available
    FS_ERR_NOATTR = -13,     // No data/attr available
    FS_ERR_NAMETOOLONG = -14 // File name too long
} fserror_t;

void fs_handler(uint8_t slot);
