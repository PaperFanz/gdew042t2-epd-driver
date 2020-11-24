/*
    source file for our reverse polish notation mode for the ePaper graphing
    calculator project

    Author: Pete Fan and Sai Koukuntla
    Date: Nov 23
*/

#include "easi.h"
#include "keymap.h"
#include "rpn.h"
#include "easi_globals.h"
#include "epdgl.h"

#include <string.h>
#include <stdint.h>

#define MAX_BUF 256
#define MAX_STACK 2

static uint32_t IN_CURSOR = 0;
static char IN_BUF[MAX_BUF];
static char STACK[MAX_STACK][MAX_BUF];

text_config_t stack_fnt = {&Consolas20, EPD_BLACK};

static void
rpn_draw_stack()
{
    epdgl_fill_rect(0, 300, 300, 60, EPD_WHITE);

    epdgl_set_cursor(20, 329);
    epdgl_draw_string("R0: ", &stack_fnt);
    epdgl_draw_string(STACK[0], &stack_fnt);

    epdgl_set_cursor(20, 309);
    epdgl_draw_string("R1: ", &stack_fnt);
    epdgl_draw_string(STACK[1], &stack_fnt);
}

static void
rpn_draw_input()
{
    epdgl_set_cursor(20, 349);
    epdgl_draw_string(IN_BUF, &stack_fnt);
}

static void
push_stack()
{
    memcpy(STACK[1], STACK[0], MAX_BUF);
    memcpy(STACK[0], IN_BUF, MAX_BUF);
    memset(IN_BUF, 0, MAX_BUF);    
}

void
rpn_init()
{
    memset(IN_BUF, 0, MAX_BUF);
    memset(STACK[0], 0, MAX_BUF);
    memset(STACK[1], 0, MAX_BUF);
    IN_CURSOR = 0;
    rpn_draw_stack();
    rpn_draw_input();
}

void
rpn_handle_input(key_t k){
    switch(k){
    case N0:
        IN_BUF[IN_CURSOR++] = 0 + '0';
        break;
    case N1:
    case N2:
    case N3:
        IN_BUF[IN_CURSOR++] = k - N1 + 1 + '0';
        break;
    case N4:
    case N5: 
    case N6:
        IN_BUF[IN_CURSOR++] = k - N4 + 4 + '0';
        break;
    case N7:
    case N8:
    case N9:
        IN_BUF[IN_CURSOR++] = k - N7 + 7 + '0';
        break;
    case DEC:
        IN_BUF[IN_CURSOR++] = '.';
        break;
    case ENTER:
        // advance the stack
        IN_CURSOR = 0;
        epdgl_fill_rect(0, 349, 300, 20, EPD_WHITE);
        push_stack();
        memset(IN_BUF, 0, MAX_BUF); 
        rpn_draw_stack();
        break;
    default:
        //pee pee poo poo
        break;
    }

    if (IN_CURSOR >= MAX_BUF){
        ERROR = 1;
    }
    rpn_draw_input();
}
