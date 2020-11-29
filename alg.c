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
char exp_string[MAX_EXPRESSION_SIZE];

//Declarations
static void alg_draw_expression(ExpressionTree *exp);

text_config_t exp_fnt = {&Consolas20, EPD_BLACK};

void alg_init(void){
	ExpressionTree_Clear(&alg_exp);
}

void alg_handle_input(key_t k){
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
			ExpressionTree_Evaluate(&alg_exp);
		default:
			return;
	}
	
	alg_draw_expression(&alg_exp);
}

void alg_clear(void){
	ExpressionTree_Clear(&alg_exp);
	
	//TODO: Clear screen
}

static void alg_draw_expression(ExpressionTree *exp){
	Expression_ToString(exp_string, alg_exp);
	
	epdgl_draw_string(exp_string, &exp_fnt);
	while (!epdgl_update_screen(EPD_FAST));
}
