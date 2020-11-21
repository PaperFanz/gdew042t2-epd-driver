/*
    simple circular fifo implementation

    Author: Pete Fan
    Date: Nov. 10
*/

#include "../inc/CortexM.h"
#include "fifo.h"

void
fifo_init (fifo_t * f)
{
    f->head = 0;
    f->tail = 0;
}

fifo_err_t
fifo_put (fifo_t * f, uint32_t i)
{
    fifo_err_t err = FIFO_NO_ERR;
    long sr = StartCritical();
    if (f->head + 1 == f->tail) {
        err = FIFO_FULL_ERR;
        goto cleanup_put;
    }
    f->head = (f->head + 1) % FIFO_SIZE;
    f->buf[f->head] = i;
cleanup_put:
    EndCritical(sr);
    return err;
}

fifo_err_t
fifo_get (fifo_t * f, uint32_t * i)
{
    fifo_err_t err = FIFO_NO_ERR;
    long sr = StartCritical();
    if (f->head == f->tail) {
        err = FIFO_EMPTY_ERR;
        goto cleanup_get;
    }
    f->tail = (f->tail + 1) % FIFO_SIZE;
    *i = f->buf[f->tail];
cleanup_get:
    EndCritical(sr);
    return err;
}
