#ifndef _EXPRESSION_TREE_H_
#define _EXPRESSION_TREE_H_

#define MAX_STACK_SIZE 20
#define MAX_EXPRESSION_SIZE 20

//#include "Globals.h"
//#include "Switch.h"
#include "keymap.h"

// Enums for ops
// normal mode
typedef enum {
	VA, VB, VC, VD, VE, VF, VG, VH, VI, VJ, VK, 
	VL, VM, VN, VO, VP, VQ, VR, VS, VT, VU, VV, 
	VW, VX, VY, VZ, 
	OPSIN, OPCOS, OPTAN, 
	OPPOW, OPLOG, OPSQRT, OPEE,  
	OPDIV, OPMUL, OPSUB, OPADD,
	OPEN_PAREN, CLOSE_PAREN,  
	NUM0, NUM1, NUM2, NUM3, NUM4, NUM5, NUM6, NUM7, NUM8, NUM9,
  POINT, SGN
} node_t;

struct ConstantStack{
	double stack[MAX_STACK_SIZE];
	int size;
}typedef ConstantStack;

struct OperatorStack{
	node_t stack[MAX_STACK_SIZE];
	int size;
}typedef OperatorStack;

struct ExpressionNodes{
	node_t nodes[MAX_STACK_SIZE];
	int size;
	int idx;
}typedef ExpressionNodes;

struct ExpressionTree{
	ExpressionNodes exp_nodes;
	
	char exp_string[30];
	double result;
	int hasResult; //0 no, 1 yes
	int validResult; //0 no, 1 yes
}typedef ExpressionTree;

//#define PRI_MAX 6
//#define PRI_MIN 0
//Lower priority has preference (0 highest priority)
enum FuncPri{ //Todo: add base
	PRI_OPEN_PAREN = 0, PRI_POW = 1, PRI_EE = 1, PRI_SIN = 2, PRI_COS = 2, PRI_TAN = 2, PRI_LOG = 2, PRI_SQRT = 2, PRI_NEG = 3, PRI_MUL = 4, PRI_DIV = 4, PRI_SUB = 5, PRI_ADD = 5, PRI_CLOSE_PAREN = 6
} typedef FuncPri;

//Initializes the expression tree to be empty
void ExpressionTree_Clear(ExpressionTree *exp);

//Modifies expression/cursor based on button pressed
int ExpressionTree_ModifyExpression(ExpressionTree *exp, key_t key);

//Evaluates the expression, places result in ExpressionTree field 
//TODO: Error handling
int ExpressionTree_Evaluate(ExpressionTree *exp);


void Expression_ToString(ExpressionTree *exp);

int ExpressionTree_Graph(ExpressionTree *exp, key_t var, double *xbuf, double *ybuf, int num_points);

#endif
