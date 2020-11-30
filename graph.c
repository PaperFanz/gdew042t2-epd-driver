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
#include "ExpressionTree.h"
#include "gui.h"

#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_BUF 21
#define NUM_WFIELDS 4
#define GRAPH_HEIGHT EPD_WIDTH - FN_BAR_H - STATUS_BAR_H
#define GRAPH_WIDTH  EPD_HEIGHT

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
static double X_AXIS[GRAPH_WIDTH];
static double Y_VAL[GRAPH_WIDTH];
static double VAL_X;
static double VAL_Y;

static uint8_t WIN_END[4];
static int8_t WIN_SEL;

static uint8_t IN_CURSOR = 0;
static uint8_t IN_END = 0;
static char IN_BUF[MAX_BUF];
static bool IN_NEG = false;
static bool EXP_VALID = false;
static bool Y_VALID = false;

static ExpressionTree graph_exp;

/*
    status variables for tracking graph mode / input
*/
static graph_mode_t G_MODE;
static bool DEC_ENTERED = false;
static bool BUF_FULL = false;
static char *END_IN_BUF;

text_config_t norm_fnt = {&Consolas20, EPD_BLACK};

static void
graph_draw_window(){
    char buf[MAX_BUF];
    epdgl_fill_rect(0, 260, 300, 100, EPD_WHITE);

    for(int i = 0; i < NUM_WFIELDS; i++){
        epdgl_set_cursor(20, 269 + 20*i);
        epdgl_draw_string(W_FNAMES[i], &norm_fnt);
        snprintf(buf, MAX_BUF, "%f", W_FVALS[i]);
        epdgl_draw_string(buf, &norm_fnt);
    }
}

static void 
graph_draw_fig(){
    // Clear
    epdgl_fill_rect(0, STATUS_BAR_H, GRAPH_WIDTH, GRAPH_HEIGHT, EPD_WHITE);

    // Draw axes
    if(W_FVALS[YMIN] <= 0 && W_FVALS[YMAX] >= 0){
        int x_ax_pix = (-1 * (W_FVALS[YMIN]) / (W_FVALS[YMAX] - W_FVALS[YMIN])) * GRAPH_HEIGHT;
        epdgl_draw_line(0, x_ax_pix, GRAPH_WIDTH, x_ax_pix, EPD_BLACK);
    }
    if(W_FVALS[XMIN] < 0 && W_FVALS[XMAX] >= 0){
        int y_ax_pix = -1 * (W_FVALS[XMIN]) / (W_FVALS[XMAX] - W_FVALS[XMIN]) * GRAPH_WIDTH;
        epdgl_draw_line(y_ax_pix, STATUS_BAR_H, y_ax_pix, EPD_WIDTH - FN_BAR_H, EPD_BLACK);
    }

    // Graph
    if(EXP_VALID){
        for(int i = 0; i < GRAPH_WIDTH - 1; i++){
            epdgl_draw_line(i, Y_VAL[i], i+1, Y_VAL[i+1], EPD_BLACK);
        }
    }
}

static void
graph_draw_input()
{
    char buf[MAX_BUF];
    uint8_t cursor_x;

    switch(G_MODE){
        case EQUATION:
            Expression_ToString(&graph_exp);
            graph_draw_fig();
            epdgl_fill_rect(20, 349, 300, 30, EPD_WHITE);    
            epdgl_set_cursor(20, 349);  
            epdgl_draw_string("Y = ", &norm_fnt);                     
            epdgl_draw_string(graph_exp.exp_string, &norm_fnt);
            break;

        case WINDOW:
            graph_draw_fig();
            cursor_x = 20 + 6 * norm_fnt.font->FixedWidth + IN_CURSOR * norm_fnt.font->FixedWidth;
            epdgl_fill_rect(10 + 6 * norm_fnt.font->FixedWidth, 269+20*WIN_SEL, 300, 20, EPD_WHITE);
            epdgl_set_cursor(20 + 6 * norm_fnt.font->FixedWidth, 269+20*WIN_SEL);
            if(IN_NEG) {
                epdgl_draw_char('-', &norm_fnt);
                cursor_x += norm_fnt.font->FixedWidth;
            }
            epdgl_draw_string(IN_BUF, &norm_fnt);
            epdgl_draw_line(cursor_x, 269+20*WIN_SEL, cursor_x, 289+20*WIN_SEL, EPD_BLACK);
            break;

        case VALUE:
            graph_draw_fig();
            epdgl_fill_rect(20, 329, 300, 50, EPD_WHITE);    
            epdgl_set_cursor(20, 329); 
            epdgl_draw_string("X = ", &norm_fnt);
            epdgl_draw_string(IN_BUF, &norm_fnt);
            epdgl_set_cursor(20, 349);
            epdgl_draw_string("Y = ", &norm_fnt);
            epdgl_fill_rect(20 + 4 * norm_fnt.font->FixedWidth, 349, 300, 20, EPD_WHITE);
            if(Y_VALID){
                snprintf(buf, MAX_BUF, "%f", VAL_Y);
                epdgl_draw_string(buf, &norm_fnt);
            }

            cursor_x = 20 + 4 * norm_fnt.font->FixedWidth + IN_CURSOR * norm_fnt.font->FixedWidth;
            if(IN_NEG) {
                epdgl_draw_char('-', &norm_fnt);
                cursor_x += norm_fnt.font->FixedWidth;
            }
            epdgl_draw_line(cursor_x, 329, cursor_x, 349, EPD_BLACK);
            break;
        default:
            break;
    }
}

static void
update_window()
{   
    if (WIN_SEL >= 0){
        //update and clamp fields, x-axis
        W_FVALS[WIN_SEL] = strtod(IN_BUF, &END_IN_BUF);
        if (IN_NEG) W_FVALS[WIN_SEL] *= -1;

        if((WIN_SEL % 2 == 0) && (W_FVALS[WIN_SEL] >= W_FVALS[WIN_SEL + 1])){
            W_FVALS[WIN_SEL] =  W_FVALS[WIN_SEL + 1] - 20;
        }
        else if ((WIN_SEL % 2 == 1) && (W_FVALS[WIN_SEL] <= W_FVALS[WIN_SEL - 1])){
            W_FVALS[WIN_SEL] =  W_FVALS[WIN_SEL - 1] + 20;
        }
        snprintf(IN_BUF, MAX_BUF, "%f", fabs(W_FVALS[WIN_SEL]));
        WIN_END[WIN_SEL] = strlen(IN_BUF);
    }

    for(int i = 0; i < GRAPH_WIDTH; i++){
        X_AXIS[i] = W_FVALS[XMIN] + (i * (W_FVALS[XMAX] - W_FVALS[XMIN])/GRAPH_WIDTH);
    }

    memset(IN_BUF, 0, MAX_BUF);    
    IN_NEG = false;
    DEC_ENTERED = false;
}

static void
update_XVal(){
    VAL_X = strtod(IN_BUF, &END_IN_BUF);
    if(IN_NEG) VAL_X *=  -1;

    if(EXP_VALID)
        Y_VALID = true;
        ExpressionTree_Graph(&graph_exp, X, &VAL_X, &VAL_Y, 1);
}

static void
graph_display(){
    switch(G_MODE){
        case FIGURE:
            break;
        case EQUATION:
            graph_draw_input();
            break;
        case WINDOW:
            graph_draw_window();
            break;
        case VALUE:
            graph_draw_input();
    }
}

void
graph_init(){
    WIN_SEL = 0;
    W_FVALS[YMIN] = W_FVALS[XMIN] = -10;
    W_FVALS[YMAX] = W_FVALS[XMAX] = 10;

    memset(IN_BUF, 0, MAX_BUF);             
    snprintf(IN_BUF, MAX_BUF, "%f", fabs(W_FVALS[YMIN]));
    WIN_END[YMIN] = WIN_END[XMIN] = strlen(IN_BUF);

    memset(IN_BUF, 0, MAX_BUF);             
    snprintf(IN_BUF, MAX_BUF, "%f", fabs(W_FVALS[YMAX]));
    WIN_END[YMAX] = WIN_END[XMAX] = strlen(IN_BUF);
    
    memset(IN_BUF, 0, MAX_BUF);   
    update_window();

    G_MODE = EQUATION;
    graph_clear();

    Y_VALID = false;
    VAL_X = 0;

    graph_display();
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

static void
graph_eqn_input(key_t k){
    bool update = true;

	switch(k){
		case N0:
		case N1:
		case N2:
		case N3:
		case N4:
		case N5:
		case N6:
		case N7:
		case N8:
		case N9:
		case DEC:
		case SIGN:
		case ADD:
		case SUB:
		case MUL:
		case DIV:
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
		case PARENTH:
		case POW:
		case EE:
		case ROOT:
		case LOG:
		case SIN:
		case COS:
		case TAN:
		case BACKSPACE:
		case LEFT:
		case RIGHT:
			ExpressionTree_ModifyExpression(&graph_exp, k);
			break;
		case ENTER:
			if (ExpressionTree_Evaluate(&graph_exp)) {
                epdgl_set_cursor(20,100);
                epdgl_draw_string("err", &norm_fnt);
                graph_clear();
            } else {
                ExpressionTree_Graph(&graph_exp, X, X_AXIS, Y_VAL, GRAPH_WIDTH);
                EXP_VALID = true;
            }
            break;
		default:
            update = false;
			break;
	}
	
    if (update) {
        graph_draw_input();
    }
}

static void
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
            update_window();
            memset(IN_BUF, 0, MAX_BUF); 
        }
        graph_draw_window();

        WIN_SEL = (WIN_SEL + 1) % NUM_WFIELDS;
        snprintf(IN_BUF, MAX_BUF, "%f", fabs(W_FVALS[WIN_SEL]));
        IN_NEG = W_FVALS[WIN_SEL] < 0;
        DEC_ENTERED = true;
        IN_CURSOR = IN_END = WIN_END[WIN_SEL];
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
    case UP:
        WIN_SEL = (WIN_SEL + 3) % NUM_WFIELDS;
        snprintf(IN_BUF, MAX_BUF, "%f", fabs(W_FVALS[WIN_SEL]));
        IN_NEG = W_FVALS[WIN_SEL] < 0;
        IN_CURSOR = IN_END = WIN_END[WIN_SEL];
        graph_draw_window();
        break;
    case DOWN:
        WIN_SEL = (WIN_SEL + 1) % NUM_WFIELDS;
        snprintf(IN_BUF, MAX_BUF, "%f", fabs(W_FVALS[WIN_SEL]));
        IN_NEG = W_FVALS[WIN_SEL] < 0;
        IN_CURSOR = IN_END = WIN_END[WIN_SEL];
        graph_draw_window();
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

static void
graph_value_input(key_t k){
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
            update_XVal();
            memset(IN_BUF, 0, MAX_BUF); 
        }
        graph_draw_window();
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
            graph_eqn_input(k);
            break;
        case WINDOW:
            graph_window_input(k);
            break;
        case VALUE:
            graph_value_input(k);
            break;
    }
  
}

static void
graph_change_mode(graph_mode_t mode){
    if(G_MODE == EQUATION){
        if(!ExpressionTree_Evaluate(&graph_exp)){
            ExpressionTree_Graph(&graph_exp, X, X_AXIS, Y_VAL, GRAPH_WIDTH);
            EXP_VALID = true;
        }
        else{
            EXP_VALID = false;
            graph_clear();
        }
    }
    G_MODE = mode;
    memset(IN_BUF, 0, MAX_BUF);
    update_window();
    graph_display();
}

void
graph_view_figure(void){
    graph_change_mode(FIGURE);
    graph_draw_fig();
}

void 
graph_set_window(void){
    WIN_SEL = 0;
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

    switch(G_MODE){
        case EQUATION:
            ExpressionTree_Clear(&graph_exp);
            memset(Y_VAL, 0, GRAPH_WIDTH);
            EXP_VALID = false;
            break;
        case WINDOW:
            memset(IN_BUF, 0, MAX_BUF);    
            IN_CURSOR = 0;
            IN_END = 0;
            IN_NEG = false;
            DEC_ENTERED = false;
            break;
        case VALUE:
            memset(IN_BUF, 0, MAX_BUF);    
            IN_CURSOR = 0;
            IN_END = 0;
            IN_NEG = false;
            DEC_ENTERED = false;
            Y_VALID = false;
        default:
            break;
    }
    graph_draw_input();

}
