/*
    source file for the Embedded Arithmetic Shell Interface (EASI)

    Author: Pete Fan
    Date: Nov 21
*/

#include "keypad.h"
#include "epdgl.h"
#include "gui.h"
#include "easi_globals.h"

void
easi_init()
{
    KEY_MODE = M_NORMAL;
    draw_status_bar();
    draw_fn_bar();
    epdgl_update_screen(EPD_FAST);
}

static void
parse_input(uint8_t raw_key)
{
    key_t key = KEYMAP[raw_key];
    switch(key){
        case ALPHA:
            if(KEY_MODE == M_ALPHA) KEY_MODE = M_NORMAL;
            else KEY_MODE = M_ALPHA;
            update_status_bar();
            break;
        case CTRL:
            if(KEY_MODE == M_CTRL) KEY_MODE = M_NORMAL;
            else KEY_MODE = M_CTRL;
            update_status_bar();
            break;
        case F1:
        case F2:
        case F3:
        case F4:
        case F5:
        case F6:
            update_fn_bar(key);
            break;
    }
}


void
easi_run()
{
    key_event_t event;
    while (key_fifo_get(&event) != FIFO_EMPTY_ERR) {
        if (event.k_action == KEY_UP) {
            parse_input(event.key);
        }
    }

    if (FULL_UPDATE) {
        epdgl_update_screen(EPD_SLOW);
        FULL_UPDATE = false;
    } else {
        epdgl_update_screen(EPD_PART);
    }
}
