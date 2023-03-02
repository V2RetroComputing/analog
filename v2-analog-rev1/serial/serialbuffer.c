#include "serial/serialbuffer.h"

volatile uint8_t *serial_reg = apple_memory + 0xC0A0;

volatile uint8_t *serial_rx_fifo = private_memory + 0;
volatile uint8_t serial_rx_wrptr;
volatile uint8_t serial_rx_rdptr;

volatile uint8_t *serial_tx_fifo = private_memory + 0x100;
volatile uint8_t serial_tx_wrptr;
volatile uint8_t serial_tx_rdptr;


void serial_rx_advance() {
    if(serial_rx_rdptr != serial_rx_wrptr) {
        serial_rx_rdptr++;
        serial_reg[8] = serial_rx_fifo[serial_rx_rdptr];
    }

    // RXFULL : 6551 has byte(s) ready from RX
    if(serial_rx_wrptr == serial_rx_rdptr) {
        serial_reg[9] &= ~ACIA_STATUS_RXFULL;
    } else {
        serial_reg[9] |= ACIA_STATUS_RXFULL;
    }
}


void serial_rx_push(uint8_t value) {
    if((serial_rx_wrptr+1) != serial_rx_rdptr) {
        serial_rx_fifo[serial_rx_wrptr] = value;
        if(serial_rx_wrptr == serial_rx_rdptr) {
            serial_reg[8] = value;
        }
        serial_rx_wrptr++;
    } else {
        serial_reg[9] |= ACIA_STATUS_RXOVER;
    }

    // RXFULL : 6551 has byte(s) ready from RX
    if(serial_rx_wrptr == serial_rx_rdptr) {
        serial_reg[9] &= ~ACIA_STATUS_RXFULL;
    } else {
        serial_reg[9] |= ACIA_STATUS_RXFULL;
    }
}

int serial_tx_empty() {
    return serial_rx_rdptr == serial_rx_wrptr;
}

uint8_t serial_tx_pop() {
    uint8_t retval;

    if(serial_rx_rdptr != serial_rx_wrptr) {
        retval = serial_rx_fifo[serial_rx_rdptr++];
    }

    // TXEMTY : 6551 able to accept byte(s) for TX
    if((serial_tx_wrptr+1) == serial_tx_rdptr) {
        serial_reg[9] &= ~ACIA_STATUS_TXEMTY;
    } else {
        serial_reg[9] |= ACIA_STATUS_TXEMTY;
    }

    return retval;
}


void serial_tx_push(uint8_t value) {
    if((serial_tx_wrptr+1) != serial_tx_rdptr) {
        serial_tx_fifo[serial_tx_wrptr] = value;
        serial_tx_wrptr++;
    }

    // TXEMTY : 6551 able to accept byte(s) for TX
    if((serial_tx_wrptr+1) == serial_tx_rdptr) {
        serial_reg[9] &= ~ACIA_STATUS_TXEMTY;
    } else {
        serial_reg[9] |= ACIA_STATUS_TXEMTY;
    }
}


void serial_fifo_init() {
    serial_rx_wrptr = 0;
    serial_rx_rdptr = 0;
    serial_tx_wrptr = 0;
    serial_tx_rdptr = 0;

    serial_reg[0x0] = 0xFF;
    serial_reg[0x1] = 0x02; // Dip Switch 2 : 8N1, No Auto CR after LF, 
    serial_reg[0x2] = 0xE0; // Dip Switch 1 : 9600 Baud Communications Mode
    serial_reg[0x3] = 0xFF;
    serial_reg[0x4] = 0xFF;
    serial_reg[0x5] = 0xFF;
    serial_reg[0x6] = 0xFF;
    serial_reg[0x7] = 0xFF;
    serial_reg[0x8] = 0x00; // Read/Write Data Register
    serial_reg[0x9] = 0x10; // Status/Reset Register
    serial_reg[0xA] = 0x0A; // Command Register
    serial_reg[0xB] = 0x1E; // Control Register
    serial_reg[0xC] = 0xFF;
    serial_reg[0xD] = 0xFF;
    serial_reg[0xE] = 0xFF;
    serial_reg[0xF] = 0xFF;
}
