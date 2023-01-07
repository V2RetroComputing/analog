#pragma once

#include <stdint.h>
#include "common/buffers.h"

extern volatile uint8_t parallel_status;
extern volatile uint8_t *parallel_tx_fifo;
extern volatile uint16_t parallel_tx_head;
extern volatile uint16_t parallel_tx_tail;

int parallel_tx_empty();
uint8_t parallel_tx_pop();
void parallel_tx_push(uint8_t value);
void parallel_fifo_init();

#define PARALLEL_BUSY     0b00001000
#define PARALLEL_PAPEROUT 0b00000100
#define PARALLEL_SELECT   0b00000010
#define PARALLEL_ACK      0b00000001

