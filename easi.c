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
#include "AlphaValues.h"

int ERROR = 0;
static bool STORE = false;

void
easi_init()
{
    KEY_MODE = M_NORMAL;
    STORE = false;
    draw_status_bar();
    draw_fn_bar();
    epdgl_update_screen(EPD_FAST);
}

static void
parse_input(uint8_t raw_key)
{
    key_t key = KEYMAP[raw_key + KEY_MODE * KEY_NUM];
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
        case STO:
            STORE = true;
            break;
		case A:
		case B:
		case C:
		case D:
		case E:
		case F:
		case G:
		case H:
		case I:
		case J:
		case K:
		case L:
		case M:
		case N:
		case O:
		case P:
		case Q:
		case R:
		case S:
		case T:
		case U:
		case V:
		case W:
		case X:
		case Y:
		case Z:
            if (STORE) {
                double val = 0.0;
                switch(EASI_MODE){
                case RPN:
                    val = rpn_get_val();
                    break;
                case VOLT:
                    
                    break;
                case ALG:
                    val = alg_get_val();
                    break;
                case GRPH:
                    val = graph_get_val();
                    break;
                default:
                    break;
                }
                set_key_alpha_value(key, val);
                STORE = false;
            }
        default:
            break;
    }

    switch(EASI_MODE){
    case RPN:
        rpn_handle_input(key);
        break;
    case VOLT:
        volt_handle_input(key);
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
    if (!SLEEP) {
        while (key_fifo_get(&event) != FIFO_EMPTY_ERR) {
            if (event.k_action == KEY_UP) {
                parse_input(event.key);
            }
        }
    }

    if (SLEEP && WAKE) {
        // wake from sleep
        SLEEP = false;

        // clear key event fifo
        while (key_fifo_get(&event) != FIFO_EMPTY_ERR);

        // reinit display
        epd_init();
        epdgl_clear();

        // give user control over new mode
        parse_input(35);
        
        // wait for full refresh before continuing
        while(!epdgl_update_screen(EPD_SLOW));
    } else if (FULL_UPDATE) {
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
    default:
        break;
    }
}
