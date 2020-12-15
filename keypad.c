/*
    source file for the key scan matrix

    Author: Pete Fan
*/

#include <stdbool.h>

#include "keypad.h"
#include "easi_globals.h"
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"

#define COL_NUM           6
#define COL_PORT_NUM      0x10
#define COL_PORT_DATA_R   GPIO_PORTE_DATA_R
#define COL_PORT_MASK     0x3F
#define COL_PORT_DIR_R    GPIO_PORTE_DIR_R
#define COL_PORT_DEN_R    GPIO_PORTE_DEN_R

#define ROW_NUM           7
#define ROW_PORT_NUM      0x02
#define ROW_PORT_DATA_R   GPIO_PORTB_DATA_R
#define ROW_PORT_MASK     0x7F
#define ROW_PORT_DIR_R    GPIO_PORTB_DIR_R
#define ROW_PORT_DEN_R    GPIO_PORTB_DEN_R

#define SYSCTL_MASK (ROW_PORT_NUM | COL_PORT_NUM)

/*
    Keyboard State Variables
*/
static uint8_t key_prev[COL_NUM];
static uint8_t key_curr[COL_NUM];

// cross-thread communication struct
typedef struct key_event_queue {
    key_event_t buf[FIFO_SIZE];
    uint32_t head;
    uint32_t tail;
} key_event_queue_t;

key_event_queue_t EVENT_QUEUE;

fifo_err_t 
key_fifo_put (key_event_t k)
{
    fifo_err_t err = FIFO_NO_ERR;
    long sr = StartCritical();
    if (EVENT_QUEUE.head + 1 == EVENT_QUEUE.tail) {
        err = FIFO_FULL_ERR;
    } else {
        EVENT_QUEUE.head = (EVENT_QUEUE.head + 1) % FIFO_SIZE;
        EVENT_QUEUE.buf[EVENT_QUEUE.head] = k;
    }
    EndCritical(sr);
    return err;
}

fifo_err_t 
key_fifo_get (key_event_t * k)
{
    fifo_err_t err = FIFO_NO_ERR;
    long sr = StartCritical();
    if (EVENT_QUEUE.head == EVENT_QUEUE.tail) {
        err = FIFO_EMPTY_ERR;
    } else {
        EVENT_QUEUE.tail = (EVENT_QUEUE.tail + 1) % FIFO_SIZE;
        *k = EVENT_QUEUE.buf[EVENT_QUEUE.tail];
    }
    EndCritical(sr);
    return err;
}

void
keypad_init(void)
{
    // Enable the clock for switch port and wait for stabilization.
    SYSCTL_RCGCGPIO_R |= SYSCTL_MASK;
    while ((SYSCTL_RCGCGPIO_R & SYSCTL_MASK) != SYSCTL_MASK);

    // Initalize row pins as digital outputs.
    COL_PORT_DIR_R |= COL_PORT_MASK;
    COL_PORT_DEN_R |= COL_PORT_MASK;

    // Initalize col pins as digital inputs.
    ROW_PORT_DIR_R &= ~(ROW_PORT_MASK);
    ROW_PORT_DEN_R = ROW_PORT_MASK;

    // Initialize event queue
    EVENT_QUEUE.head = 0;
    EVENT_QUEUE.tail = 0;
}

void
keypad_scan(void)
{
    int col, row;
    key_event_t ev;

    for (col = 0; col < COL_NUM; ++col) {
        COL_PORT_DATA_R = (COL_PORT_DATA_R & ~COL_PORT_MASK) | (0x01 << col);

        key_prev[col] = key_curr[col];

        key_curr[col] = ROW_PORT_DATA_R & ROW_PORT_MASK;

        uint8_t diff = key_prev[col] ^ key_curr[col];
        for (row = 0; row < ROW_NUM; ++row) {
            if (diff & (0x01 << row)) {
                ev.key = (col * ROW_NUM) + row;
                if (key_prev[col] & (0x01 << row)) {
                    ev.k_action = KEY_UP;
                } else {
                    ev.k_action = KEY_DN;
                }
                key_fifo_put(ev);
                WAKE = true;
            }
        }
    }

    COL_PORT_DATA_R = (COL_PORT_DATA_R & ~COL_PORT_MASK);
}

uint8_t * 
keypad_get_raw(void)
{
    return key_curr;
}
