
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

typedef signed char int8_t;
typedef signed short int16_t;

#define PICOPAL_REGISTER(n) (*(volatile unsigned char *)(0xC080 | (n) | (cardslot << 4)))

#define PICOPAL_WRDATA    PICOPAL_REGISTER(0x0)
#define PICOPAL_ALSB      PICOPAL_REGISTER(0x1)
#define PICOPAL_AMSB      PICOPAL_REGISTER(0x2)
#define PICOPAL_MODE      PICOPAL_REGISTER(0x3)
#define PICOPAL_TBCOLOR   PICOPAL_REGISTER(0x4)
#define PICOPAL_BORDER    PICOPAL_REGISTER(0x5)
#define PICOPAL_RDDATA    PICOPAL_REGISTER(0x7)
#define PICOPAL_WRFONT    PICOPAL_REGISTER(0x8)
#define PICOPAL_RDFONT    PICOPAL_REGISTER(0xf)

#define CFG_BUFFER ((volatile unsigned char *)(0xC0E0 | (cardslot << 8)))

typedef enum {
    MODE_REBOOT = 0,
    MODE_DIAG,
    MODE_FS,
    MODE_VGACARD,
    MODE_APPLICARD,
    MODE_SERIAL,
    MODE_PARALLEL,
    MODE_MIDI,
    MODE_SNESMAX,
    MODE_ETHERNET
} v2mode_t;

typedef enum {
    SERIAL_LOOP = 0,
    SERIAL_USB,
    SERIAL_WIFI,
    SERIAL_PRINTER,
} serialmux_t;

typedef enum {
    USB_HOST_CDC,
    USB_GUEST_CDC,
    USB_GUEST_MIDI,
} usbmux_t;

typedef enum {
    WIFI_CLIENT = 0,
    WIFI_AP,
} wifimode_t;


#define FS_BUFFER ((volatile unsigned char *)(0xC800))

#define FS_COMMAND        PICOPAL_REGISTER(0x0)
#define FS_FILE           PICOPAL_REGISTER(0x1)
#define FS_FLAGS          PICOPAL_REGISTER(0x1)
#define FS_SIZELSB        PICOPAL_REGISTER(0x2)
#define FS_SIZEMSB        PICOPAL_REGISTER(0x3)
#define FS_OFFLSB         PICOPAL_REGISTER(0x2)
#define FS_OFFMSB         PICOPAL_REGISTER(0x3)
#define FS_WHENCE         PICOPAL_REGISTER(0x4)
#define FS_BUSY           PICOPAL_REGISTER(0xD)
#define FS_STATUS         PICOPAL_REGISTER(0xE)
#define FS_EXECUTE        PICOPAL_REGISTER(0xF)


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

typedef enum {
    MACHINE_AUTO = 0,
    MACHINE_APPLE_II = 1,
    MACHINE_APPLE_IIE = 2,
    MACHINE_APPLE_IIGS = 3,
    MACHINE_MAXVALUE = 4
} compat_t;
