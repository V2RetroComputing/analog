#include "parallel/parallelbuffer.h"

volatile uint8_t parallel_status = ~PARALLEL_SELECT;
volatile uint8_t *parallel_tx_fifo = private_memory;
volatile uint16_t parallel_tx_wrptr = 0;
volatile uint16_t parallel_tx_rdptr = 0;


int parallel_tx_empty() {
    return (parallel_tx_wrptr == parallel_tx_rdptr);
}


uint8_t parallel_tx_pop() {
    uint8_t retval;
    if(parallel_tx_rdptr != parallel_tx_wrptr) {
        retval = parallel_tx_fifo[parallel_tx_rdptr];
        parallel_tx_rdptr++;
    }

    // Assert BUSY LOW when buffer is full
    if((parallel_tx_wrptr+1) == parallel_tx_rdptr) {
        parallel_status &= ~PARALLEL_BUSY;
    } else {
        parallel_status |= PARALLEL_BUSY;
    }

    return retval;
}


void parallel_tx_push(uint8_t value) {
    if((parallel_tx_wrptr+1) != parallel_tx_rdptr) {
        parallel_tx_fifo[parallel_tx_wrptr] = value;
        parallel_tx_wrptr++;
    }

    // Deassert BUSY LOW when buffer is not full
    if((parallel_tx_wrptr+1) == parallel_tx_rdptr) {
        parallel_status &= ~PARALLEL_BUSY;
    } else {
        parallel_status |= PARALLEL_BUSY;
    }

    // Assert ACK
    parallel_status &= ~PARALLEL_ACK;
}


void parallel_fifo_init() {
    parallel_tx_wrptr = 0;
    parallel_tx_rdptr = 0;
    parallel_status = ~PARALLEL_SELECT;
}
