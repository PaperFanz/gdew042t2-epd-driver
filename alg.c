/*
    source file for alg mode for the ePaper graphing
    calculator project

    Author: Ethan Golla
    Date: Nov 29
*/

#include "alg.h"
#include "easi.h"
#include "easi_globals.h"
#include "epdgl.h"

#include "ExpressionTree.h"

#include <stdint.h>

//Globals
ExpressionTree alg_exp;

ExpressionTree history[5];
static int history_size = 0;

//Declarations
static void alg_draw_expression(ExpressionTree *exp, int loc_on_screen);
static void alg_draw(void);
void alg_update_history(ExpressionTree *exp);

text_config_t exp_fnt = {&Consolas20, EPD_BLACK};

static bool NEW_EXPRESSION = true;

void alg_init(void){
    NEW_EXPRESSION = true;
}

void alg_handle_input(key_t k){
    bool update = true;

    if (NEW_EXPRESSION) {
        alg_clear();
        NEW_EXPRESSION = false;
    }

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
			ExpressionTree_ModifyExpression(&alg_exp, k);
			break;
		case ENTER:
			if (ExpressionTree_Evaluate(&alg_exp)) {
                epdgl_set_cursor(20,100);
                epdgl_draw_string("err", &exp_fnt);
            } else {
                NEW_EXPRESSION = true;
            }
            break;
		default:
            update = false;
			break;
	}
	
    if (update) {
        alg_draw_expression(&alg_exp, 5); //New expression always bottommost location
    }
}

void alg_clear(void){
	ExpressionTree_Clear(&alg_exp);
	
	//TODO: Clear screen
    epdgl_fill_rect(20, 20, 280, 20, EPD_WHITE);    // clear off area
}

//loc_on_screen: 0 top, 5 bottom
static void alg_draw_expression(ExpressionTree *exp, int loc_on_screen){
    static uint16_t py = 20;
	Expression_ToString(&alg_exp);
	
    epdgl_fill_rect(20, py, 280, 20, EPD_WHITE);    // clear off area
    epdgl_set_cursor(20, py);                       // set cursor coordinates
	epdgl_draw_string(exp->exp_string, &exp_fnt);        // draw string
    py += 20;
}

static void alg_draw(void){
	alg_draw_expression(&alg_exp, 5);

	int hist_idx = history_size - 1;
	while(hist_idx >= 0){
		alg_draw_expression(&history[hist_idx], hist_idx);
		
		hist_idx--;
	}
}

void alg_update_history(ExpressionTree *exp){
	if(history_size < 5){
		history[history_size] = *exp;
		history_size++;
	}
	else{
		for(int i = 1; i < 5; i++){
			history[i - 1] = history[i];
		}
		history[4] = *exp;
	}
	
	alg_draw();
}
