#pragma once

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
    MODE_DIAG,
    MODE_VGACARD,
    MODE_APPLICARD,
    MODE_SERIAL,
    MODE_PARALLEL,
    MODE_SNESMAX,
    MODE_ETHERNET
} v2mode_t;

extern v2mode_t v2mode;

typedef enum {
    SERIAL_LOOP = 0,
    SERIAL_USB,
    SERIAL_WIFI,
    SERIAL_PRINTER,
} serialmux_t;

extern serialmux_t serialmux;

typedef enum {
    USB_HOST_CDC,
    USB_GUEST_CDC,
    USB_GUEST_MIDI,
} usbmux_t;

extern usbmux_t usbmux;

typedef enum {
    WIFI_CLIENT = 0,
    WIFI_AP,
} wifimode_t;

extern wifimode_t wifimode;

enum {
    ABUS_MAIN_SM = 0,
    ABUS_DEVICE_READ_SM = 1,
};

#define CARD_SELECT   ((value & (1u << CONFIG_PIN_APPLEBUS_DEVSEL-CONFIG_PIN_APPLEBUS_DATA_BASE)) == 0)
#define CARD_DEVSEL   ((address & 0xcf80) == 0xc080)
#define CARD_IOSEL    (((address & 0xcf00) >= 0xc100) && ((address & 0xcf00) < 0xc700))
#define CARD_IOSTROBE ((address & 0xc800) == 0xc800)

void read_config();
void write_config();
void config_handler();
void default_config();
