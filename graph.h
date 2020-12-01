/*
    header file for graphing mode for the ePaper graphing
    calculator project

    Author: Sai Koukuntla
    Date: Nov 28
*/

#ifndef GRAPH_H
#define GRAPH_H

#include "keymap.h"

void graph_init(void);

void graph_handle_input(key_t k);

void graph_view_figure(void);

void graph_set_window(void);

void graph_set_eqn(void);

void graph_trace(void);

double graph_get_val(void);

void graph_clear(void);

#endif
