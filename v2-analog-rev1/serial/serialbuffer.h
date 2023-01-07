#pragma once

#include <stdint.h>
#include "common/buffers.h"

extern uint8_t ssc_rom[2048];

extern volatile uint8_t *serial_reg;

void serial_rx_advance();
void serial_rx_push(uint8_t value);

int serial_tx_empty();
uint8_t serial_tx_pop();
void serial_tx_push(uint8_t value);
void serial_fifo_init();

#define ACIA_STATUS_PARITY 0b00000001
#define ACIA_STATUS_FRAMNG 0b00000010
#define ACIA_STATUS_RXOVER 0b00000100
#define ACIA_STATUS_RXFULL 0b00001000
#define ACIA_STATUS_TXEMTY 0b00010000
#define ACIA_STATUS_DCD    0b00100000
#define ACIA_STATUS_DSR    0b01000000
#define ACIA_STATUS_IRQ    0b10000000
