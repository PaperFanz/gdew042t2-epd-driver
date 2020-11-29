/*
    source file for graphing mode for the ePaper graphing
    calculator project

    Author: Sai Koukuntla
    Date: Nov 28
*/

#include "easi.h"
#include "keymap.h"
#include "graph.h"
#include "easi_globals.h"
#include "epdgl.h"

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_BUF 21
#define NUM_WFIELDS 4

/*
    typedefs
*/
typedef enum graph_mode{
    FIGURE,
    EQUATION,
    WINDOW,
    VALUE
} graph_mode_t;

typedef enum win_fields{
    YMIN,
    YMAX,
    XMIN,
    XMAX
} win_field_t;

/*
    constants
*/
const static char *W_FNAMES[] = {"yMin: ", "yMax: ", "xMin: ", "xMax: "};

/*
    globals for tracking window/graph status
*/
static double W_FVALS[] = {0, 0, 0, 0};
static double X_AXIS[EPD_HEIGHT];

static uint8_t WIN_END[4];
static int8_t WIN_SEL;

static uint8_t IN_CURSOR = 0;
static uint8_t IN_END = 0;
static char IN_BUF[MAX_BUF];
static bool IN_NEG = false;

// Equation 1 - however we store this
// Equation 2 - however we store this

/*
    status variables for tracking graph mode / input
*/
static graph_mode_t G_MODE;
static bool DEC_ENTERED = false;
static bool BUF_FULL = false;
static char *END_IN_BUF;

text_config_t norm_fnt = {&Consolas20, EPD_BLACK};
text_config_t sel_fnt = {&Consolas20, EPD_WHITE};

static void
graph_draw_window(){
    char buf[MAX_BUF];
    epdgl_fill_rect(0, 260, 300, 100, EPD_WHITE);

    for(int i = 0; i < NUM_WFIELDS; i++){
        text_config_t fnt = (i == WIN_SEL) ? sel_fnt : norm_fnt;
        if(i == WIN_SEL){
            epdgl_fill_rect(18, 269+20*WIN_SEL, (6 + WIN_END[WIN_SEL]) * 14 + 2, 20, EPD_BLACK);
        }

        epdgl_set_cursor(20, 269 + 20*i);
        epdgl_draw_string(W_FNAMES[i], &fnt);
        snprintf(buf, MAX_BUF, "%f", W_FVALS[i]);
        epdgl_draw_string(buf, &fnt);
    }
}

static void
graph_draw_input()
{
    uint8_t cursor_x = 20 + IN_CURSOR * norm_fnt.font->FixedWidth;
    epdgl_fill_rect(0, 349, 300, 20, EPD_WHITE);
    epdgl_set_cursor(20, 349);
    if(IN_NEG) {
        epdgl_draw_char('-', &norm_fnt);
        cursor_x += norm_fnt.font->FixedWidth;
    }
    epdgl_draw_string(IN_BUF, &norm_fnt);
    epdgl_draw_line(cursor_x, 349, cursor_x, 369, EPD_BLACK);
}

static void
update_window()
{   
    //update and clamp fields, x-axis
    W_FVALS[WIN_SEL] = strtod(IN_BUF, &END_IN_BUF);
    if (IN_NEG) W_FVALS[WIN_SEL] *= -1;

    if(W_FVALS[WIN_SEL % 2] && (W_FVALS[WIN_SEL] >= W_FVALS[WIN_SEL + 1])){
        W_FVALS[WIN_SEL] =  W_FVALS[WIN_SEL + 1] - 20;
    }
    else if (W_FVALS[WIN_SEL] <= W_FVALS[WIN_SEL - 1]){
        W_FVALS[WIN_SEL] =  W_FVALS[WIN_SEL - 1] + 20;
    }
    snprintf(IN_BUF, MAX_BUF, "%f", fabs(W_FVALS[WIN_SEL]));
    WIN_END[WIN_SEL] = strlen(IN_BUF);

    for(int i = 0; i < EPD_HEIGHT; i++){
        X_AXIS[i] = W_FVALS[XMIN] + (i * (W_FVALS[XMAX] - W_FVALS[XMIN])/EPD_HEIGHT);
    }

    memset(IN_BUF, 0, MAX_BUF);    
    IN_NEG = false;
}

static void
graph_display(){
    switch(G_MODE){
        case FIGURE:
            break;
        case EQUATION:
            break;
        case WINDOW:
            graph_draw_window();
            break;
        case VALUE:
            break;
    }
}

void
graph_init(){
    G_MODE = WINDOW;
    WIN_SEL = -1;
    W_FVALS[YMIN] = W_FVALS[XMIN] = -10;
    W_FVALS[YMAX] = W_FVALS[XMAX] = 10;
    WIN_END[YMIN] = WIN_END[XMIN] = 3;
    WIN_END[XMIN] = WIN_END[XMAX] = 2;
    update_window();
}

void
graph_append_num(int8_t num)
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
graph_window_input(key_t k){
    switch(k){
    case N0:
        graph_append_num(0);
        break;
    case N1:
    case N2:
    case N3:
        graph_append_num(k - N1 + 1);
        break;
    case N4:
    case N5: 
    case N6:
        graph_append_num(k - N4 + 4);
        break;
    case N7:
    case N8:
    case N9:
        graph_append_num(k - N7 + 7);
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
        if(IN_CURSOR && WIN_SEL != -1){
            IN_CURSOR = 0;
            IN_END = 0;
            BUF_FULL = false;
            DEC_ENTERED = false;
            epdgl_fill_rect(0, 349, 300, 20, EPD_WHITE);
            update_window();
            memset(IN_BUF, 0, MAX_BUF); 
            graph_draw_window();
        }
        WIN_SEL = (WIN_SEL + 1) % NUM_WFIELDS;
        snprintf(IN_BUF, MAX_BUF, "%f", fabs(W_FVALS[WIN_SEL]));
        IN_NEG = W_FVALS[WIN_SEL] < 0;
        IN_CURSOR = IN_END = WIN_END[WIN_SEL];
        break;
    case BACKSPACE:
        if (IN_CURSOR) {
            --IN_CURSOR;
            --IN_END;
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
    case UP:
        WIN_SEL = (WIN_SEL + 3) % NUM_WFIELDS;
        snprintf(IN_BUF, MAX_BUF, "%f", fabs(W_FVALS[WIN_SEL]));
        IN_NEG = W_FVALS[WIN_SEL] < 0;
        IN_CURSOR = IN_END = WIN_END[WIN_SEL];
        break;
    case DOWN:
        WIN_SEL = (WIN_SEL + 1) % NUM_WFIELDS;
        snprintf(IN_BUF, MAX_BUF, "%f", fabs(W_FVALS[WIN_SEL]));
        IN_NEG = W_FVALS[WIN_SEL] < 0;
        IN_CURSOR = IN_END = WIN_END[WIN_SEL];
        break;
    case SIGN:
        IN_NEG = !IN_NEG;
        break;
    default:
        break;
    }

    if (IN_END >= MAX_BUF){
        ERROR = 1;
        BUF_FULL = true;
    }
    graph_draw_input();
}

void 
graph_handle_input(key_t k){
    switch(G_MODE){
        case FIGURE:
            break;
        case EQUATION:
            break;
        case WINDOW:
            graph_window_input(k);
            break;
        case VALUE:
            break;
    }
  
}

static void
graph_change_mode(graph_mode_t mode){
    G_MODE = mode;
    memset(IN_BUF, 0, MAX_BUF);
    graph_display();
}

void 
graph_set_window(void){
    WIN_SEL = -1;
    graph_change_mode(WINDOW);
}

void 
graph_set_eqn(void){
    graph_change_mode(EQUATION);
}

void 
graph_get_val(void){
    graph_change_mode(VALUE);
}

void 
graph_clear(void){
  
}
