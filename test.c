/*
    source file for alg mode for the ePaper graphing
    calculator project

    Author: Ethan Golla
    Date: Dec 2
*/

#include "ExpressionTree.h"
#include "keymap.h"
#include "test.h"

test_t testArr[NUM_TEST_CASES] = {
	//Basic test of each operation
	{{{{NUM1, OPADD, NUM2}, 							3, 0}, "", 0, 0, 0}, 1, 3},//1 + 2
	{{{{NUM3, OPSUB, NUM6}, 							3, 0}, "", 0, 0, 0}, 1, -3}, //3 - 6
	{{{{NUM1, POINT, NUM3, OPMUL, NUM2}, 	5, 0}, "", 0, 0, 0}, 1, 2.6}, //1.3 * 2
	{{{{NUM4, POINT, NUM5, OPDIV, NUM5}, 	5, 0}, "", 0, 0, 0}, 1, .9}, //4.5 / 5
	
	{{{{OPEE, NUM0}, 											2, 0}, "", 0, 0, 0}, 1, 1},//e^0
	{{{{NUM8, OPPOW, NUM2}, 							3, 0}, "", 0, 0, 0}, 1, 64}, //8^2
	{{{{OPLOG, NUM1, NUM0, NUM0}, 				4, 0}, "", 0, 0, 0}, 1, 2}, //log(100)
	{{{{OPSQRT, NUM8, NUM1}, 							3, 0}, "", 0, 0, 0}, 1, 9}, //sqrt(81)
	
	{{{{OPCOS, NUM0}, 										2, 0}, "", 0, 0, 0}, 1, 1},//cos(0)
	{{{{OPSIN, NUM0}, 										2, 0}, "", 0, 0, 0}, 1, 0},//sin(0)
	{{{{OPTAN, NUM0}, 										2, 0}, "", 0, 0, 0}, 1, 0},//tan(0)
	
	{{{{SGN, NUM1, NUM7}, 								3, 0}, "", 0, 0, 0}, 1, -17},//-17
	
	//Error detection
	{{{{OPCOS}, 																1, 0}, "", 0, 0, 0}, 0, 0},	//ERROR: No operand
	{{{{NUM3, OPSIN, NUM0}, 										3, 0}, "", 0, 0, 0}, 0, 0}, //ERROR: No implicit multiplication for constants, too many operands
	{{{{NUM4, OPDIV, NUM0}, 										3, 0}, "", 0, 0, 0}, 0, 0}, //ERROR: Div by 0 
	{{{{OPSQRT, SGN, NUM9}, 										3, 0}, "", 0, 0, 0}, 0, 0}, //ERROR: Sqrt negative number
	
	//Order of operations
	{{{{NUM1, OPADD, NUM0, OPMUL, NUM2}, 														5, 0}, "", 0, 0, 0}, 1, 1},//1 + 0 * 2
	{{{{NUM1, OPSUB, NUM0, OPDIV, NUM2}, 														5, 0}, "", 0, 0, 0}, 1, 1},//1 - 0 / 2
	{{{{OPEN_PAREN, NUM1, OPADD, NUM0, CLOSE_PAREN, OPMUL, NUM2},		7, 0}, "", 0, 0, 0}, 1, 2},//(1 + 0) * 2
};

//0 for all test cases passed, 1 for a failed test case
//idxs of failed tests are in testFailed
//numTestsFailed has the amount of failed test cases
int test_alg(int* testsFailed, int* numTestsFailed){
	*numTestsFailed = 0;
	for(int i = 0; i < NUM_TEST_CASES; i++){
		ExpressionTree_Evaluate(&(testArr[i].exp));
		if((testArr[i].exp.validResult != testArr[i].validResult) || (testArr[i].exp.result != testArr[i].expectedResult)){
			testsFailed[*numTestsFailed] = i;
			*numTestsFailed = *numTestsFailed + 1;
		}
	}
	
	if(*numTestsFailed){
		return 1;
	}
	else{
		return 0;
	}
}	
