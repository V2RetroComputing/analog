#pragma once

#include <stdint.h>

extern uint8_t flash_control;
extern uint8_t flash_status;
extern uint8_t flash_data;
extern uint32_t flash_address;

enum {
    FLASH_STATUS_READY = 0x00,
    FLASH_STATUS_BUSY  = 0x01,
    FLASH_STATUS_ECMD  = 0xE0, // ERROR: Unknown Command
    FLASH_STATUS_EPRM  = 0xE2, // ERROR: Incorrect Parameter
    FLASH_STATUS_ECRC  = 0xE4, // ERROR: Invalid Checksum

    FLASH_CONTROL_IDLE   = 0x00,
    FLASH_CONTROL_ERASE  = 0x45,
    FLASH_CONTROL_UPLOAD = 0x55,
    FLASH_CONTROL_REBOOT = 0x52,
};
