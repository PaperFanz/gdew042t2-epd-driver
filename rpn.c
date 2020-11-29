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
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_BUF 21
#define MAX_STACK 2

/*
    globals for tracking rpn stack
*/
static uint8_t IN_CURSOR = 0;
static uint8_t IN_END = 0;
static bool NEG[MAX_STACK];
static char STACK[MAX_STACK][MAX_BUF];
static double DSTACK[MAX_STACK];

/*
    status variables for tracking stack entry
*/
static bool DEC_ENTERED = false;
static bool BUF_FULL = false;
static bool IN_NEG = false;
static char IN_BUF[MAX_BUF];
static char *END_IN_BUF;

text_config_t stack_fnt = {&Consolas20, EPD_BLACK};

static void
rpn_draw_stack()
{
    epdgl_fill_rect(0, 300, 300, 60, EPD_WHITE);

    epdgl_set_cursor(20, 329);
    epdgl_draw_string("R0: ", &stack_fnt);
    if (NEG[0]) epdgl_draw_char('-', &stack_fnt);
    epdgl_draw_string(STACK[0], &stack_fnt);

    epdgl_set_cursor(20, 309);
    epdgl_draw_string("R1: ", &stack_fnt);
    if (NEG[1]) epdgl_draw_char('-', &stack_fnt);
    epdgl_draw_string(STACK[1], &stack_fnt);
}

static void
rpn_draw_input()
{
    uint8_t cursor_x = 20 + IN_CURSOR * stack_fnt.font->FixedWidth;
    epdgl_fill_rect(0, 349, 300, 20, EPD_WHITE);
    epdgl_set_cursor(20, 349);
    if(IN_NEG) {
        epdgl_draw_char('-', &stack_fnt);
        cursor_x += stack_fnt.font->FixedWidth;
    }
    epdgl_draw_string(IN_BUF, &stack_fnt);
    epdgl_draw_line(cursor_x, 349, cursor_x, 369, EPD_BLACK);
}

static void
push_stack()
{
    memcpy(STACK[1], STACK[0], MAX_BUF);
    NEG[1] = NEG[0];
    DSTACK[1] = DSTACK[0];
    memcpy(STACK[0], IN_BUF, MAX_BUF);
    NEG[0] = IN_NEG;
    DSTACK[0] = strtod(IN_BUF, &END_IN_BUF);
    if (IN_NEG) DSTACK[0] *= -1;
    memset(IN_BUF, 0, MAX_BUF);    
    IN_NEG = false;
}

void
rpn_init()
{
    memset(IN_BUF, 0, MAX_BUF);
    memset(STACK[0], 0, MAX_BUF);
    memset(STACK[1], 0, MAX_BUF);
    IN_NEG = false;
    NEG[0] = false; NEG[1] = false;
    DSTACK[0] = 0.0; DSTACK[1] = 0.0;
    IN_CURSOR = 0;
    IN_END = 0;
    rpn_draw_stack();
    rpn_draw_input();
}

void
rpn_update_output()
{
    snprintf(STACK[0], MAX_BUF, "%f", fabs(DSTACK[0]));
    rpn_draw_stack();
}

void
rpn_append_num(int8_t num)
{
    if (!BUF_FULL) {
        if(IN_CURSOR != IN_END){
            memmove(&IN_BUF[IN_CURSOR + 1], &IN_BUF[IN_CURSOR], IN_END - IN_CURSOR);
        }
        IN_BUF[IN_CURSOR] = num + '0';
        IN_CURSOR++;
        IN_END++;
    }
}

void
rpn_handle_input(key_t k){
    switch(k){
    case N0:
        rpn_append_num(0);
        break;
    case N1:
    case N2:
    case N3:
        rpn_append_num(k - N1 + 1);
        break;
    case N4:
    case N5: 
    case N6:
        rpn_append_num(k - N4 + 4);
        break;
    case N7:
    case N8:
    case N9:
        rpn_append_num(k - N7 + 7);
        break;
    case DEC:
        if (!DEC_ENTERED && !BUF_FULL) {
            if(IN_CURSOR != IN_END){
                memmove(&IN_BUF[IN_CURSOR + 1], &IN_BUF[IN_CURSOR], IN_END - IN_CURSOR);
            }
            IN_BUF[IN_CURSOR++] = '.';
            IN_END++;
            DEC_ENTERED = true;
        }
        break;
    case ENTER:
        // advance the stack
        if(IN_CURSOR){
            IN_CURSOR = 0;
            IN_END = 0;
            BUF_FULL = false;
            DEC_ENTERED = false;
            epdgl_fill_rect(0, 349, 300, 20, EPD_WHITE);
            push_stack();
            memset(IN_BUF, 0, MAX_BUF); 
            rpn_draw_stack();
        }
        break;
    case BACKSPACE:
        if (IN_CURSOR) {
            --IN_CURSOR;
            --IN_END;
            if(IN_BUF[IN_CURSOR] == '.') DEC_ENTERED = false;
            memmove(&IN_BUF[IN_CURSOR], &IN_BUF[IN_CURSOR + 1], IN_END - IN_CURSOR);
            IN_BUF[IN_END] = 0;
        }
        break;
    case LEFT:
        if (IN_CURSOR) {
            --IN_CURSOR;
        }
        break;
    case RIGHT:
        if(IN_CURSOR < IN_END) {
            ++IN_CURSOR;
        }
        break;
    case SIGN:
        IN_NEG = !IN_NEG;
        break;
    case ADD:
        DSTACK[0] = DSTACK[1] + DSTACK[0];
        NEG[0] = DSTACK[0] < 0;
        rpn_update_output();
        break;
    case SUB:
        DSTACK[0] = DSTACK[1] - DSTACK[0];
        NEG[0] = DSTACK[0] < 0;
        rpn_update_output();
        break;
    case MUL:
        DSTACK[0] = DSTACK[1] * DSTACK[0];
        NEG[0] = DSTACK[0] < 0;
        rpn_update_output();
        break;
    case DIV:
        if(DSTACK[0]){
            DSTACK[0] = DSTACK[1] / DSTACK[0];
            NEG[0] = DSTACK[0] < 0;
            rpn_update_output();
        } else {
            ERROR = 1;
        }
        break;
    case POW:
        DSTACK[0] = pow(DSTACK[0], DSTACK[1]);
        NEG[0] = DSTACK[0] < 0;
        rpn_update_output();
        break;
    case LOG:
        if (DSTACK[0] > 0) {
            DSTACK[0] = log(DSTACK[0]);
            NEG[0] = DSTACK[0] < 0;
            rpn_update_output();
        } else {
            ERROR = 1;
        }
        break;
    case SIN:
        DSTACK[0] = sin(DSTACK[0]);
        NEG[0] = DSTACK[0] < 0;
        rpn_update_output();
        break;
    case COS:
        DSTACK[0] = cos(DSTACK[0]);
        NEG[0] = DSTACK[0] < 0;
        rpn_update_output();
        break;
    case TAN:
        DSTACK[0] = tan(DSTACK[0]);
        NEG[0] = DSTACK[0] < 0;
        rpn_update_output();
        break;
    case ROOT:
        DSTACK[0] = sqrt(DSTACK[0]);
        NEG[0] = DSTACK[0] < 0;
        rpn_update_output();
        break;
    default:
        break;
    }

    if (IN_END >= MAX_BUF){
        ERROR = 1;
        BUF_FULL = true;
    }
    rpn_draw_input();
}

void
rpn_clear(){
    IN_NEG = false;
    IN_CURSOR = 0;
    IN_END = 0;
    memset(IN_BUF, 0, MAX_BUF); 
}

void 
rpn_swap(){
    char temp[MAX_BUF];
    double dtemp;
    bool ntemp;

    memcpy(temp, STACK[0], MAX_BUF);
    dtemp = DSTACK[0];
    ntemp = NEG[0];
    memcpy(STACK[0], STACK[1], MAX_BUF);
    DSTACK[0] = DSTACK[1];
    NEG[0] = NEG[1];
    memcpy(STACK[1], temp, MAX_BUF);
    DSTACK[1] = dtemp;
    NEG[1] = ntemp;

    rpn_draw_stack();
}

