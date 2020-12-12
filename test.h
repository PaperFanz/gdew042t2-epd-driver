/*
    header file for shell globals

    Author: Ethan Golla
    Date: Dec 2
*/

#ifndef TEST_H
#define TEST_H

#include "ExpressionTree.h"

#define NUM_TEST_CASES 19

struct test_t{
	ExpressionTree exp;
	double validResult;
	double expectedResult;
} typedef test_t;

int test_alg(int* testsFailed, int* numTestsFailed);

#endif
