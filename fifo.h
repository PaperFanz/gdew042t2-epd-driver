/*
    simple circular fifo implementation

    Author: Pete Fan
    Date: Nov. 10
*/

#ifndef _FIFO_H_
#define _FIFO_H_

#include <stdint.h>

#define FIFO_SIZE 32

typedef enum fifoErr {
    FIFO_NO_ERR,
    FIFO_EMPTY_ERR,
    FIFO_FULL_ERR
} fifo_err_t;

typedef struct Fifo {
    uint32_t buf[FIFO_SIZE];
    uint32_t head;
    uint32_t tail;
} fifo_t;

void fifo_init (fifo_t * f);

fifo_err_t fifo_put (fifo_t * f, uint32_t i);

fifo_err_t fifo_get (fifo_t * f, uint32_t * i);

#endif
