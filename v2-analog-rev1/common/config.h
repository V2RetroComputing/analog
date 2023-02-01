#pragma once

#include <stdint.h>

#define CONFIG_SYSCLOCK 126.0 /* MHz */

// Pin configuration
#define CONFIG_PIN_APPLEBUS_DATA_BASE 0 /* 8+2 pins */
#define CONFIG_PIN_APPLEBUS_DEVSEL (CONFIG_PIN_APPLEBUS_DATA_BASE+8)
#define CONFIG_PIN_APPLEBUS_RW     (CONFIG_PIN_APPLEBUS_DATA_BASE+9)
#define CONFIG_PIN_APPLEBUS_CONTROL_BASE 10 /* 4 pins */
#define CONFIG_PIN_APPLEBUS_PHI0 26
#define CONFIG_PIN_HSYNC 28
#define CONFIG_PIN_VSYNC 27
#define CONFIG_PIN_RGB_BASE 14 /* 9 pins */

// Other resources
#define CONFIG_VGA_PIO pio0
#define CONFIG_VGA_SPINLOCK_ID 31
#define CONFIG_ABUS_PIO pio1

typedef enum {
    MODE_REBOOT = 0,
    MODE_UNKNOWN,
    MODE_DIAG,
    MODE_FS,
    MODE_VGACARD,
    MODE_APPLICARD,
    MODE_SERIAL,
    MODE_PARALLEL,
    MODE_SNESMAX,
    MODE_ETHERNET
} v2mode_t;

extern volatile v2mode_t cfg_mode;
extern volatile v2mode_t current_mode;

typedef enum {
    SERIAL_LOOP = 0,
    SERIAL_USB,
    SERIAL_WIFI,
    SERIAL_PRINTER,
} serialmux_t;

extern volatile serialmux_t serialmux;

typedef enum {
    USB_HOST_CDC,
    USB_GUEST_CDC,
    USB_GUEST_MIDI,
} usbmux_t;

extern volatile usbmux_t usbmux;

typedef enum {
    WIFI_CLIENT = 0,
    WIFI_AP,
} wifimode_t;

extern volatile wifimode_t wifimode;

typedef enum {
    MACHINE_II = 0,
    MACHINE_IIE = 1,
    MACHINE_IIGS = 2,
    MACHINE_PRAVETZ = 6,
    MACHINE_AGAT7 = 7,
    MACHINE_AGAT9 = 9,
    MACHINE_AUTO = 0xff
} compat_t;

extern volatile compat_t cfg_machine;
extern volatile compat_t current_machine;

enum {
    ABUS_MAIN_SM = 0,
    ABUS_DEVICE_READ_SM = 1,
};

#define CARD_SELECT   ((value & (1u << CONFIG_PIN_APPLEBUS_DEVSEL-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)
#define CARD_DEVSEL   ((address & 0xcf80) == 0xc080)
#define CARD_IOSEL    (((address & 0xcf00) >= 0xc100) && ((address & 0xcf00) < 0xc700))
#define CARD_IOSTROBE ((address & 0xc800) == 0xc800)

int make_config(uint8_t *buf, uint16_t len);
void read_config();
void write_config();
void config_handler();
void default_config();

