/*
    header file for the key scan matrix

    Author: Pete Fan

    Hardware:
        scan columns: PE5-0
        scan rows: PB6-0
*/

#ifndef _KEYPAD_H_
#define _KEYPAD_H_

#include <stdint.h>

#include "keymap.h"
#include "fifo.h"

typedef enum key_action {
    KEY_DN, // keypress (low -> hi)
    KEY_UP  // keyrelease (hi->low)
} key_action_t;

typedef struct key_event {
    uint8_t key;
    key_action_t k_action;
} key_event_t;

void keypad_init(void);

void keypad_scan(void);

uint8_t * keypad_get_raw(void);

fifo_err_t key_fifo_get (key_event_t * k);

#endif
