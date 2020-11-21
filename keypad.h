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

#include "fifo.h"

typedef enum key {
    F1,      F2,    F3,   F4,   F5,   F6,    
             ENTER, MOD1, MOD2, UP,   BACKSPACE,     
    SIN,     COS,   TAN,  LEFT, DOWN, RIGHT,    
    CONST,   VAR,   POW,  LOG,  ROOT,   
    EE,      N7,    N8,   N9,   DIV,        
    PARENTH, N4,    N5,   N6,   MUL,       
    BASE,    N1,    N2,   N3,   SUB,      
    MATH,    N0,    DEC,  SIGN, ADD,
    KEY_NUM,
    /* alpha mode */
    A,       B,      C,
    D,       E,      F,     G,      H, 
    I,       J,      K,     L,      M, 
    N,       O,      P,     Q,      R, 
    S,       T,      U,     V,      W, 
    X,       Y,      Z,     SPACE,  SHIFT,
    /* mode 1 */
    // TODO
    /* mode 2 */
    // TODO
    INVALID_KEY
} key_t;

typedef enum key_action {
    KEY_NO,
    KEY_DN, // keypress (low -> hi)
    KEY_UP  // keyrelease (hi->low)
} key_action_t;

typedef struct key_event {
    key_t key;
    key_action_t k_action;
} key_event_t;

void keypad_init(void);

void keypad_scan(void);

uint8_t * keypad_get_raw(void);

fifo_err_t key_fifo_get (key_event_t * k);

#endif
