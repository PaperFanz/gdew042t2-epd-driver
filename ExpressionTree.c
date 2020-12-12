// Main.c
// Runs on TM4C123
// The Squad
// October 23, 2020

#include <stdint.h>
#include <stdio.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#include "ExpressionTree.h"
#include "Math.h"
#include "AlphaValues.h"

//Assumes valid input
double node_to_constant(node_t node){
	switch(node){
		case NUM0:
			return 0;
		case NUM1:
			return 1;
		case NUM2:
			return 2;
		case NUM3:
			return 3;
		case NUM4:
			return 4;
		case NUM5:
			return 5;
		case NUM6:
			return 6;
		case NUM7:
			return 7;
		case NUM8:
			return 8;
		case NUM9:
			return 9;
		case VA:
		case VB:
		case VC:
		case VD:
		case VE:
		case VF:
		case VG:
		case VH:
		case VI:
		case VJ:
		case VK:
		case VL:
		case VM:
		case VN:
		case VO:
		case VP:
		case VQ:
		case VR:
		case VS:
		case VT:
		case VU:
		case VV:
		case VW:
		case VX:
		case VY:
		case VZ:
			return get_node_alpha_value(node);
		default: 
			return 404; //input error
	}
}

//0 success, 1 failure
int execute_operation(double *result, double src1, double src2, node_t op){
	int error;
	switch(op){
		case OPADD:
			error = Math_Add(result, src1, src2);
			break;
		case OPSUB:
			error = Math_Sub(result, src1, src2);
			break;
		case OPMUL:
			error = Math_Mul(result, src1, src2);	
			break;
		case OPDIV:
			error = Math_Div(result, src1, src2);
			break;
		case OPCOS:
			error = Math_Cos(result, src1);
			break;
		case OPSIN:
			error = Math_Sin(result, src1);
			break;
		case OPTAN:
			error = Math_Tan(result, src1);	
			break;
		case OPLOG:
			error = Math_Log10(result, src1);
			break;
		case OPSQRT:
			error = Math_Sqrt(result, src1);
			break;
		case OPEE:
			error = Math_ExponentE(result, src1);
			break;
		case OPPOW:
			error = Math_Pow(result, src1, src2);
			break;
		case SGN:
			error = Math_Neg(result, src1);
			break;
		default:
			return 1; //error
	}
	
	return error;
}

void ExpressionTree_Clear(ExpressionTree *exp){
	exp->hasResult = 0;
	exp->exp_nodes.idx = 0;
	exp->exp_nodes.size = 0;
}

/*
//0 success, 1 failure
int num_nodes_for_key(int *result, key_t key){
	switch(key){
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
		case ADD:
		case MUL:
		case SUB:
		case DIV:
		case DEC:
		case SIGN:
			*result = 1;
			break;
		case PARENTH:
			*result = 2;
			break;
		case POW:
		case EE:
		case ROOT:
		case LOG:
		case SIN:
		case COS:
		case TAN:
			*result = 3;
			break;
		default:
			return 1;
	}
	
	return 0;
}
*/


//node_t nodeArr[3];

//idxptroffset is to move cursor within parentheses
//size is amount of nodes
//nodeArr is where the nodes are
//0 success, 1 failure
int convert_key_to_nodes(node_t *nodeArr, int *size, int *idxPtrOffset, key_t key){
	*size = 0;
	*idxPtrOffset = 0;
	
	switch(key){
		case N0:
			nodeArr[0] = NUM0;
			*size = 1;
			break;
		case N1:
			nodeArr[0] = NUM1;
			*size = 1;
			break;
		case N2:
			nodeArr[0] = NUM2;
			*size = 1;
			break;
		case N3:
			nodeArr[0] = NUM3;
			*size = 1;
			break;
		case N4:
			nodeArr[0] = NUM4;
			*size = 1;
			break;
		case N5:
			nodeArr[0] = NUM5;
			*size = 1;
			break;
		case N6:
			nodeArr[0] = NUM6;
			*size = 1;
			break;
		case N7:
			nodeArr[0] = NUM7;
			*size = 1;
			break;
		case N8:
			nodeArr[0] = NUM8;
			*size = 1;
			break;
		case N9:
			nodeArr[0] = NUM9;
			*size = 1;
			break;
		case DEC:
			nodeArr[0] = POINT;
			*size = 1;
			break;
		case SIGN:
			nodeArr[0] = SGN;
			*size = 1;
			break;
		case ADD:
			nodeArr[0] = OPADD;
			*size = 1;
			break;
		case SUB:
			nodeArr[0] = OPSUB;
			*size = 1;
			break;
		case MUL:
			nodeArr[0] = OPMUL;
			*size = 1;
			break;
		case DIV:
			nodeArr[0] = OPDIV;
			*size = 1;
			break;
		case A:
			nodeArr[0] = VA;
			*size = 1;
			break;
		case B:
			nodeArr[0] = VB;
			*size = 1;
			break;
		case C:
			nodeArr[0] = VC;
			*size = 1;
			break;
		case D:
			nodeArr[0] = VD;
			*size = 1;
			break;
		case E:
			nodeArr[0] = VE;
			*size = 1;
			break;
		case F:
			nodeArr[0] = VF;
			*size = 1;
			break;
		case G:
			nodeArr[0] = VG;
			*size = 1;
			break;
		case H:
			nodeArr[0] = VH;
			*size = 1;
			break;
		case I:
			nodeArr[0] = VI;
			*size = 1;
			break;
		case J:
			nodeArr[0] = VJ;
			*size = 1;
			break;
		case K:
			nodeArr[0] = VK;
			*size = 1;
			break;
		case L:
			nodeArr[0] = VL;
			*size = 1;
			break;
		case M:
			nodeArr[0] = VM;
			*size = 1;
			break;
		case N:
			nodeArr[0] = VN;
			*size = 1;
			break;
		case O:
			nodeArr[0] = VO;
			*size = 1;
			break;
		case P:
			nodeArr[0] = VP;
			*size = 1;
			break;
		case Q:
			nodeArr[0] = VQ;
			*size = 1;
			break;
		case R:
			nodeArr[0] = VR;
			*size = 1;
			break;
		case S:
			nodeArr[0] = VS;
			*size = 1;
			break;
		case T:
			nodeArr[0] = VT;
			*size = 1;
			break;
		case U:
			nodeArr[0] = VU;
			*size = 1;
			break;
		case V:
			nodeArr[0] = VV;
			*size = 1;
			break;
		case W:
			nodeArr[0] = VW;
			*size = 1;
			break;
		case X:
			nodeArr[0] = VX;
			*size = 1;
			break;
		case Y:
			nodeArr[0] = VY;
			*size = 1;
			break;
		case Z:
			nodeArr[0] = VZ;
			*size = 1;
			break;
		case PARENTH:
			nodeArr[0] = OPEN_PAREN;
			nodeArr[1] = CLOSE_PAREN;
			*size = 2;	
			*idxPtrOffset = -1;
			break;
		case POW:
			nodeArr[0] = OPPOW;
			nodeArr[1] = OPEN_PAREN;
			nodeArr[2] = CLOSE_PAREN;
			*size = 3;	
			*idxPtrOffset = -1;
			break;
		case EE:
			nodeArr[0] = OPEE;
			nodeArr[1] = OPEN_PAREN;
			nodeArr[2] = CLOSE_PAREN;
			*size = 3;	
			*idxPtrOffset = -1;
			break;
		case ROOT:
			nodeArr[0] = OPSQRT;
			nodeArr[1] = OPEN_PAREN;
			nodeArr[2] = CLOSE_PAREN;
			*size = 3;	
			*idxPtrOffset = -1;
			break;
		case LOG:
			nodeArr[0] = OPLOG;
			nodeArr[1] = OPEN_PAREN;
			nodeArr[2] = CLOSE_PAREN;
			*size = 3;	
			*idxPtrOffset = -1;
			break;
		case SIN:
			nodeArr[0] = OPSIN;
			nodeArr[1] = OPEN_PAREN;
			nodeArr[2] = CLOSE_PAREN;
			*size = 3;	
			*idxPtrOffset = -1;
			break;
		case COS:
			nodeArr[0] = OPCOS;
			nodeArr[1] = OPEN_PAREN;
			nodeArr[2] = CLOSE_PAREN;
			*size = 3;	
			*idxPtrOffset = -1;
			break;
		case TAN:
			nodeArr[0] = OPTAN;
			nodeArr[1] = OPEN_PAREN;
			nodeArr[2] = CLOSE_PAREN;
			*size = 3;	
			*idxPtrOffset = -1;
			break;
		default:
			return 1;
	} 
	return 0;
}

//0 success, 1 failure
int add_node(ExpressionTree *exp, key_t key){
	//int num_ops = ExpressionTree_NumOps(button);
	int num_nodes;
	int idx_offset;
	
	node_t newNodesArr[3]; //TODO: 3 should be a constant for max nodes
	if(convert_key_to_nodes(newNodesArr, &num_nodes, &idx_offset, key)) return 1;
	
	//If inserting in the middle, shift nodes on the right over to make room
	if(exp->exp_nodes.idx < exp->exp_nodes.size){
		for(int i = exp->exp_nodes.size - 1 + num_nodes; i > exp->exp_nodes.idx; i--){
			exp->exp_nodes.nodes[i] = exp->exp_nodes.nodes[i - num_nodes];
		}
	}
	
	//Place new nodes
	for(int i = 0; i < num_nodes; i++){
		exp->exp_nodes.nodes[exp->exp_nodes.idx] = newNodesArr[i];
		exp->exp_nodes.size++;
		exp->exp_nodes.idx++;
	}
	
	//Correct cursor if parentheses
	exp->exp_nodes.idx += idx_offset;
	
	return 0;
}

//0 success, 1 failure
int ExpressionTree_ModifyExpression(ExpressionTree *exp, key_t key){
	switch(key){
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
			add_node(exp, key);
			break;
		case BACKSPACE:
			if(exp->exp_nodes.idx > 0){
				for(int i = exp->exp_nodes.idx; i < exp->exp_nodes.size; i++){
					exp->exp_nodes.nodes[i - 1] = exp->exp_nodes.nodes[i];
				}
				
				exp->exp_nodes.idx--;
				exp->exp_nodes.size--;
			}
			break;
		case LEFT:
			if(exp->exp_nodes.idx > 0){
				exp->exp_nodes.idx--;
			}
			break;
		case RIGHT:
			if(exp->exp_nodes.idx < exp->exp_nodes.size){
				exp->exp_nodes.idx++;
			}
			break;
		default:
			return 1; //Nothing was done, not necessarrily an error
	}
	return 0;
}

//-1 if bad input
//Returns the number of inputs for a node operation
int num_inputs_for_opnode(node_t node){
	if(node == OPADD){
		return 2;
	}
	else if(node == OPSUB){
		return 2;		
	}
	else if(node == OPMUL){
		return 2;		
	}
	else if(node == OPDIV){
		return 2;		
	}
	else if(node == OPCOS){
		return 1;		
	}
	else if(node == OPSIN){
		return 1;		
	}
	else if(node == OPTAN){
		return 1;		
	}
	else if(node == OPLOG){
		return 1;		
	}
	else if(node == OPSQRT){
		return 1;		
	}
	else if(node == OPEE){
		return 1;		
	}
	else if(node == OPPOW){
		return 2;		
	}
	else if(node == SGN){
		return 1;
	}
	else{
		return -1;
	}	
}

//-1 if not found
int opnode_priority(node_t node){
	switch(node){
		case OPADD:
			return PRI_ADD;
		case OPSUB:
			return PRI_SUB;		
		case OPMUL:
			return PRI_MUL;	
		case OPDIV:
			return PRI_DIV;
		case OPCOS:
			return PRI_COS;		
		case OPSIN:
			return PRI_SIN;		
		case OPTAN:
			return PRI_TAN;
		case OPLOG:
			return PRI_LOG;		
		case OPSQRT:
			return PRI_SQRT;		
		case OPPOW:
			return PRI_POW;
		case OPEE:
			return PRI_EE;		
		case OPEN_PAREN:
			return PRI_OPEN_PAREN;		
		case CLOSE_PAREN:
			return PRI_CLOSE_PAREN;
		case SGN:
			return PRI_NEG;
		default:
			return -1;
	}
}

//1 yes, 0 no, -1 error
int calculate_prev_opnode(node_t current, node_t previous){
	int currPri = opnode_priority(current);
	int prevPri = opnode_priority(previous);
	
	if(currPri == -1 || prevPri == -1) return -1;
	
	if(previous == OPEN_PAREN && current != CLOSE_PAREN){
		return 0;
	}
	
	if(prevPri <= currPri){
		return 1;
	}
	else{
		return 0;
	}
}

void expression_invalid(ExpressionTree *exp){
	exp->hasResult = 1;
	exp->validResult = 0;
	
	exp->exp_nodes.size--; //Remove closing parenthesis
}

//0 success, 1 failure
int ExpressionTree_Evaluate(ExpressionTree *exp){
	int evalConstant = 0; //false
	double constantVal = 0;
	
	int divisor = 10;
	int hitDecimal = 0; //false

	OperatorStack opStack;
	ConstantStack constStack;
	
	opStack.stack[0] = OPEN_PAREN; //Opening parenthesis in op stack
	opStack.size = 1;
    constStack.size = 0;
	
	exp->exp_nodes.nodes[exp->exp_nodes.size] = CLOSE_PAREN; //Add a closing paren to expression
	exp->exp_nodes.size++;
	
	for(int i = 0; i < exp->exp_nodes.size; i++){
		//key_t button = expressionTree.expression.buttons[i];
		node_t node = exp->exp_nodes.nodes[i];
		
		if(node == NUM0 || 
			 node == NUM1 || 
			 node == NUM2 || 
			 node == NUM3 ||
			 node == NUM4 ||
			 node == NUM5 ||
			 node == NUM6 ||
		   node == NUM7 ||
			 node == NUM8 ||
		   node == NUM9 ||
			 node == POINT)
		{
			
			evalConstant = 1; 
			if(node == POINT){
				if(hitDecimal){
					expression_invalid(exp);
					return 1; //2 decimal error
				}
				hitDecimal = 1;
			}
			else{
				if(hitDecimal == 0){
					constantVal = (constantVal * 10) + node_to_constant(node);
				}
				else{
					constantVal += (node_to_constant(node) / divisor);
					divisor *= 10;
				}
			}
		}	
		else if(evalConstant == 1){ //Pushes a constant
			constStack.stack[constStack.size] = constantVal;
			constStack.size++;
			
			//Resetting constant logic
			evalConstant = 0;
			constantVal = 0;
			divisor = 10;
			hitDecimal = 0;
		}
		
		//TODO: error with a constant and a variable bordering
		if(node == VA || node == VB || node == VC || node == VD || node == VE || node == VF || node == VG || node == VH || node == VI ||
			 node == VJ || node == VK || node == VL || node == VM || node == VN || node == VO || node == VP || node == VQ || node == VR ||
		   node == VS || node == VT || node == VU || node == VV || node == VW || node == VX || node == VY || node == VZ){
			
			constStack.stack[constStack.size] = node_to_constant(node);
			constStack.size++;
		}
		
		if(node == OPADD || node == OPSUB || node == OPMUL || node == OPDIV || 
			 node == OPSIN || node == OPCOS || node == OPTAN || 
		   node == OPEE || node == OPPOW || node == OPLOG || node == OPSQRT || 
		   node == OPEN_PAREN || node == CLOSE_PAREN || node == SGN)
		{										
			
			if(opStack.size == 0){
				expression_invalid(exp);
				return 1;
			}
			node_t prevOp = opStack.stack[opStack.size - 1];
				
			int perform_operation = calculate_prev_opnode(node, prevOp);									
										
			while(perform_operation){
				opStack.size--;
						
				if(prevOp == OPEN_PAREN){
					break;
				}
						
				double src1;
				double src2;
				double res;
				int numInputs = num_inputs_for_opnode(prevOp);
				if(numInputs >= 2){
					if(constStack.size == 0){
						expression_invalid(exp);
						return 1;
					}
					constStack.size--;
					src2 = constStack.stack[constStack.size];
				}
				else{
					src2 = 0;
				}
				if(numInputs >= 1){
					if(constStack.size == 0){
						expression_invalid(exp);
						return 1;
					}
					constStack.size--;
					src1 = constStack.stack[constStack.size];
				}
				else{
					src1 = 0;
				}
							
				if(execute_operation(&res, src1, src2, prevOp)){ 
					expression_invalid(exp);
					return 1; 
				}
					
				constStack.stack[constStack.size] = res;
				constStack.size++;
						
				if(opStack.size == 0){
					expression_invalid(exp);
					return 1;
				}
				prevOp = opStack.stack[opStack.size - 1];
				perform_operation = calculate_prev_opnode(node, prevOp);
			}

			if(node != CLOSE_PAREN){
				opStack.stack[opStack.size] = node;
				opStack.size++;	
			}
		}
	}
	
	if(constStack.size != 1 || opStack.size != 0){
		expression_invalid(exp);
		return 1;
	}
	
	exp->result = constStack.stack[0];
	exp->hasResult = 1;
	exp->validResult = 1;
	
	exp->exp_nodes.size--; //Remove closing parenthesis TODO: For errors this has to be done as well
		
	return 0;
}
	

//No size check
void Expression_ToString(ExpressionTree *exp){
	//Clear string
	for(int i = 0; i < 30; i++){
		exp->exp_string[i] = 0;
	}
	
	int string_idx = 0;
	for(int i = 0; i < exp->exp_nodes.size; i++){
		if(i == exp->exp_nodes.idx && exp->hasResult == 0){
			exp->exp_string[string_idx] = '|';
			string_idx++;
		}
			
		node_t node = exp->exp_nodes.nodes[i];
		char node_char = ' ';
		switch(node){
			case VA:
				node_char = 'A';
				break;
			case VB:
				node_char = 'B';
				break;
			case VC:
				node_char =	'C';
				break;
			case VD:
				node_char =	'D';	
				break;
			case VE:
				node_char =	'E';
				break;
			case VF:
				node_char =	'F';
				break;
			case VG:
				node_char =	'G';
				break;
			case VH:
				node_char =	'H';
				break;
			case VI:
				node_char =	'I';
				break;
			case VJ:
				node_char =	'J';
				break;
			case VK:
				node_char =	'K';
				break;
			case VL:
				node_char =	'L';		
				break;
			case VM:
				node_char =	'M';	
				break;
			case VN:
				node_char =	'N';
				break;
			case VO:
				node_char =	'O';
				break;
			case VP:
				node_char =	'P';
				break;
			case VQ:
				node_char =	'Q';
				break;
			case VR:
				node_char =	'R';
				break;
			case VS:
				node_char =	'S';
				break;
			case VT:
				node_char =	'T';
				break;
			case VU:
				node_char =	'U';
				break;
			case VV:
				node_char =	'V';
				break;
			case VW:
				node_char =	'W';
				break;
			case VX:
				node_char =	'X';
				break;
			case VY:
				node_char =	'Y';
				break;
			case VZ:
				node_char =	'Z';
				break;
			case NUM0:
				node_char =	'0';
				break;
			case NUM1:
				node_char =	'1';
				break;
			case NUM2:
				node_char =	'2';
				break;
			case NUM3:
				node_char =	'3';
				break;
			case NUM4:
				node_char =	'4';
				break;
			case NUM5:
				node_char =	'5';
				break;
			case NUM6:
				node_char =	'6';
				break;
			case NUM7:
				node_char =	'7';
				break;
			case NUM8:
				node_char =	'8';
				break;
			case NUM9:
				node_char =	'9';
				break;
			case OPADD:
				node_char =	'+';
				break;
			case OPSUB:
				node_char =	'-';
				break;
			case OPMUL:
				node_char =	'*';
				break;
			case OPDIV:
				node_char =	'/';
				break;
			case OPSIN:
				exp->exp_string[string_idx] = 's';
				string_idx++;
				exp->exp_string[string_idx] = 'i';
				string_idx++;
				node_char =	'n';
				break;
			case OPCOS:
				exp->exp_string[string_idx] = 'c';
				string_idx++;
				exp->exp_string[string_idx] = 'o';
				string_idx++;
				node_char =	's';
				break;
			case OPTAN:
				exp->exp_string[string_idx] = 't';
				string_idx++;
				exp->exp_string[string_idx] = 'a';
				string_idx++;
				node_char =	'n';
				break;
			case OPPOW:
				node_char =	'^';
				break;
			case OPEE:
				exp->exp_string[string_idx] = 'e';
				string_idx++;
				node_char =	'^';
				break;
			case OPLOG:
				exp->exp_string[string_idx] = 'l';
				string_idx++;
				exp->exp_string[string_idx] = 'o';
				string_idx++;
				node_char =	'g';
				break;
			case OPSQRT:
				exp->exp_string[string_idx] = 's';
				string_idx++;
				exp->exp_string[string_idx] = 'q';
				string_idx++;
				exp->exp_string[string_idx] = 'r';
				string_idx++;
				node_char =	't';
				break;
			case OPEN_PAREN:
				node_char =	'(';
				break;
			case CLOSE_PAREN:
				node_char =	')';
				break;
			case POINT:
				node_char =	'.';
				break;
			case SGN:
				node_char =	'-';
				break;
			default:
				break;
		}
		
		exp->exp_string[string_idx] = node_char;
		string_idx++;
	}
	
	if(exp->hasResult == 1){
		exp->exp_string[string_idx] = ' ';
		string_idx++;
		exp->exp_string[string_idx] = '=';
		string_idx++;
		exp->exp_string[string_idx] = ' ';
		string_idx++;
		
		if(exp->validResult == 1){
			double result = exp->result;
			if(result < 0){
				exp->exp_string[string_idx] = '-';
				string_idx++;
				result = -result;
			}
			
			
			char result_str[20];
			snprintf(result_str, 20, "%f", result);
			
			//Remove zero padding
			int temp_idx = 0;
			while(result_str[temp_idx] != '\0'){
				temp_idx++;
			}
			
			char letter = result_str[temp_idx - 1];
			for(int i = temp_idx - 1; letter == '0'; i--){
				letter = result_str[i];
				if(letter == '0' || letter == '.')
					result_str[i] = '\0';
			}
			
			//Translate result string to end of expression string
			int result_str_idx = 0;
			while(result_str[result_str_idx] != '\0'){
				exp->exp_string[string_idx] = result_str[result_str_idx];
				string_idx++;
				
				result_str_idx++;
			}
		}
		else{
			char syn_err_str[6] = "Error";
			for(int i = 0; i < 6; i++){
				exp->exp_string[string_idx] = syn_err_str[i];
				string_idx++;
			}
		}
	}
	else if(exp->exp_nodes.idx == exp->exp_nodes.size){ //Cursor placement at end
		exp->exp_string[string_idx] = '|';
		string_idx++;
	}
	
	exp->exp_string[string_idx] = '\0';
}

void Expression_String(ExpressionTree *exp){
	//Clear string
	for(int i = 0; i < 30; i++){
		exp->exp_string[i] = 0;
	}
	
	int string_idx = 0;
	for(int i = 0; i < exp->exp_nodes.size; i++){		
		node_t node = exp->exp_nodes.nodes[i];
		char node_char = ' ';
		switch(node){
			case VA:
				node_char = 'A';
				break;
			case VB:
				node_char = 'B';
				break;
			case VC:
				node_char =	'C';
				break;
			case VD:
				node_char =	'D';	
				break;
			case VE:
				node_char =	'E';
				break;
			case VF:
				node_char =	'F';
				break;
			case VG:
				node_char =	'G';
				break;
			case VH:
				node_char =	'H';
				break;
			case VI:
				node_char =	'I';
				break;
			case VJ:
				node_char =	'J';
				break;
			case VK:
				node_char =	'K';
				break;
			case VL:
				node_char =	'L';		
				break;
			case VM:
				node_char =	'M';	
				break;
			case VN:
				node_char =	'N';
				break;
			case VO:
				node_char =	'O';
				break;
			case VP:
				node_char =	'P';
				break;
			case VQ:
				node_char =	'Q';
				break;
			case VR:
				node_char =	'R';
				break;
			case VS:
				node_char =	'S';
				break;
			case VT:
				node_char =	'T';
				break;
			case VU:
				node_char =	'U';
				break;
			case VV:
				node_char =	'V';
				break;
			case VW:
				node_char =	'W';
				break;
			case VX:
				node_char =	'X';
				break;
			case VY:
				node_char =	'Y';
				break;
			case VZ:
				node_char =	'Z';
				break;
			case NUM0:
				node_char =	'0';
				break;
			case NUM1:
				node_char =	'1';
				break;
			case NUM2:
				node_char =	'2';
				break;
			case NUM3:
				node_char =	'3';
				break;
			case NUM4:
				node_char =	'4';
				break;
			case NUM5:
				node_char =	'5';
				break;
			case NUM6:
				node_char =	'6';
				break;
			case NUM7:
				node_char =	'7';
				break;
			case NUM8:
				node_char =	'8';
				break;
			case NUM9:
				node_char =	'9';
				break;
			case OPADD:
				node_char =	'+';
				break;
			case OPSUB:
				node_char =	'-';
				break;
			case OPMUL:
				node_char =	'*';
				break;
			case OPDIV:
				node_char =	'/';
				break;
			case OPSIN:
				exp->exp_string[string_idx] = 's';
				string_idx++;
				exp->exp_string[string_idx] = 'i';
				string_idx++;
				node_char =	'n';
				break;
			case OPCOS:
				exp->exp_string[string_idx] = 'c';
				string_idx++;
				exp->exp_string[string_idx] = 'o';
				string_idx++;
				node_char =	's';
				break;
			case OPTAN:
				exp->exp_string[string_idx] = 't';
				string_idx++;
				exp->exp_string[string_idx] = 'a';
				string_idx++;
				node_char =	'n';
				break;
			case OPPOW:
				node_char =	'^';
				break;
			case OPEE:
				exp->exp_string[string_idx] = 'e';
				string_idx++;
				node_char =	'^';
				break;
			case OPLOG:
				exp->exp_string[string_idx] = 'l';
				string_idx++;
				exp->exp_string[string_idx] = 'o';
				string_idx++;
				node_char =	'g';
				break;
			case OPSQRT:
				exp->exp_string[string_idx] = 's';
				string_idx++;
				exp->exp_string[string_idx] = 'q';
				string_idx++;
				exp->exp_string[string_idx] = 'r';
				string_idx++;
				node_char =	't';
				break;
			case OPEN_PAREN:
				node_char =	'(';
				break;
			case CLOSE_PAREN:
				node_char =	')';
				break;
			case POINT:
				node_char =	'.';
				break;
			case SGN:
				node_char =	'-';
				break;
			default:
				break;
		}
		
		exp->exp_string[string_idx] = node_char;
		string_idx++;
	}
	
	exp->exp_string[string_idx] = '\0';
}

//0 success, 1 failure
int ExpressionTree_Graph(ExpressionTree *exp, key_t var, double *xbuf, double *ybuf, int num_points){
	double var_value = get_key_alpha_value(var);
	
	for(int i = 0; i < num_points; i++){
		set_key_alpha_value(var, xbuf[i]);
		if(ExpressionTree_Evaluate(exp)) {
			set_key_alpha_value(var, var_value); //Reset variable value
			return 1;
		}
		ybuf[i] = exp->result;
	}
	
	set_key_alpha_value(var, var_value); //Reset variable value
	return 0;
}
