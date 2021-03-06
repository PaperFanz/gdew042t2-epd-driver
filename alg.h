/*
    header file for algebraic calculation

    Author: Ethan Golla
    Date: Nov 29
*/

#ifndef _ALG_H_
#define _ALG_H_

#include "keymap.h"

void alg_init(void);

void alg_handle_input(key_t k);

void alg_clear(void);

double alg_get_val(void);

void alg_ans(void);

void alg_volt(void);

#endif
