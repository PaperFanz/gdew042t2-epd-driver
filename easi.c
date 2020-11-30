/*
    source file for the Embedded Arithmetic Shell Interface (EASI)

    Author: Pete Fan
    Date: Nov 21
*/

#include "alg.h"
#include "keypad.h"
#include "epdgl.h"
#include "gui.h"
#include "rpn.h"
#include "easi_globals.h"
#include "voltmeter.h"
#include "graph.h"

int ERROR = 0;

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

    switch(EASI_MODE){
    case RPN:
        rpn_handle_input(key);
        break;
    case VOLT:
        break;
    case ALG:
        alg_handle_input(key);
        break;
    case GRPH:
        graph_handle_input(key);
        break;
    default:
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

    switch(EASI_MODE){
    case RPN:
        break;
    case VOLT:
        draw_voltmeter();
        break;
		case ALG:
				break;
    default:
        break;
    }

    if (FULL_UPDATE) {
        epdgl_update_screen(EPD_SLOW);
        FULL_UPDATE = false;
    } else {
        epdgl_update_screen(EPD_PART);
    }
}

void 
clear_input(){
    switch(EASI_MODE){
        case RPN:
            rpn_clear();
            break;
				case ALG:
						alg_clear();
            break;
        case GRPH:
            graph_clear();
            break;
    }
}
