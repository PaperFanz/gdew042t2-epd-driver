/*
    header file for our reverse polish notation mode for the ePaper graphing
    calculator project

    Author: Pete Fan and Sai Koukuntla
    Date: Nov 23
*/

#ifndef RPN_H
#define RPN_H

#include "keymap.h"

void rpn_init(void);

void rpn_handle_input(key_t k);

void rpn_clear(void);

void rpn_swap(void);

double rpn_get_val(void);

#endif
