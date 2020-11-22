/*
    defines possible key inputs and key matrix mapping from 
    raw keycodes to key_t

    Author: Pete Fan
    Date: Nov 21
*/

#ifndef KEYMAP_H
#define KEYMAP_H

typedef enum key {
    F1,      F2,    F3,    F4,   F5,   F6,    
             ENTER, ALPHA, CTRL, UP,   BACKSPACE,     
    SIN,     COS,   TAN,   LEFT, DOWN, RIGHT,    
    CONST,   VAR,   POW,   LOG,  ROOT,   
    EE,      N7,    N8,    N9,   DIV,        
    PARENTH, N4,    N5,    N6,   MUL,       
    BASE,    N1,    N2,    N3,   SUB,      
    MATH,    N0,    DEC,   SIGN, ADD,
    KEY_NUM,
    /* alpha mode */
    A,       B,      C,
    D,       E,      F,     G,      H, 
    I,       J,      K,     L,      M, 
    N,       O,      P,     Q,      R, 
    S,       T,      U,     V,      W, 
    X,       Y,      Z,     SPACE,  SHIFT,
    /* control mode */
    // TODO
    INVALID_KEY
} key_t;

extern const key_t KEYMAP[];

#endif
