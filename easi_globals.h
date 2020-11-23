/*
    header file for shell globals

    Author: Pete Fan
    Date: Nov 21
*/

#ifndef EASI_GLOBALS_H
#define EASI_GLOBALS_H

#include <stdbool.h>

/*
    shell status variables
*/
typedef enum easi_mode {
    ALG,
    RPN,
    GRPH,
    VOLT,
    PP,
} easi_mode_t;

extern easi_mode_t EASI_MODE;

typedef enum key_mode {
    M_NORMAL,
    M_ALPHA,
    M_CTRL
} key_mode_t;

extern key_mode_t KEY_MODE;

extern bool FULL_UPDATE;

#endif
