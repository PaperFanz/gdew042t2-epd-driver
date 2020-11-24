/*
    easi graphical interface header file

    Authors: Pete Fan and Sai Koukuntla
    Date: Nov 22
*/

#ifndef GUI_H
#define GUI_H

#include "keypad.h"

// portrait mode
#define SCREEN_H 400
#define SCREEN_W 300

#define FN_BAR_W 300
#define FN_BAR_H 20

#define STATUS_BAR_W 300
#define STATUS_BAR_H 20

void draw_fn_bar(void);

void draw_status_bar(void);

void update_status_bar(void);

void update_fn_bar(key_t k);

#endif
