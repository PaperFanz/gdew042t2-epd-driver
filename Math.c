// Math.c
// Runs on TM4C123
// The Squad
// October 23, 2020

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#include "Math.h"

int Math_Add(double *res, double src1, double src2){
	*res = src1 + src2;
	return 0;
}

int Math_Sub(double *res, double src1, double src2){
	*res = src1 - src2;
	return 0;
}

int Math_Mul(double *res, double src1, double src2){
	*res = src1 * src2;
	return 0;
}

int Math_Div(double *res, double src1, double src2){
	if(src2 == 0) return 1;
	
	*res = src1 / src2;
	return 0;
}

int Math_Sin(double *res, double src1){
	*res = sin(src1);
	return 0;
}

int Math_Cos(double *res, double src1){
	*res = cos(src1);
	return 0;
}

//TODO: Test tan pi/2
int Math_Tan(double *res, double src1){
	*res = tan(src1);
	return 0;
}

//unimplemented
int Math_Cot(double *res, double src1){
	return 1 / (tan(src1));
}

//unimplemented
int Math_Csc(double *res, double src1){
	return 1/(sin(src1));
}

//unimplemented
int Math_Sec(double *res, double src1){
	return 1 / (cos(src1));
}

int Math_ExponentE(double *res, double src1){ //e^x
	*res = exp(src1);
	return 0;
}

//unimplemented
int Math_ArcSin(double *res, double src1){
	return asin(src1);
}

//unimplemented
int Math_ArcCos(double *res, double src1){
	return acos(src1);
}

//unimplemented
int Math_ArcTan(double *res, double src1){
	return atan(src1);
}

//unimplemented
int Math_ArcCot(double *res, double src1){ //These next 3 might be wrong
	return 1 / (atan(src1));
}

//unimplemented
int Math_ArcCsc(double *res, double src1){
	return 1/(asin(src1));
}

//unimplemented
int Math_ArcSec(double *res, double src1){
	return 1 / (acos(src1));
}

//unimplemented
int Math_Cosh(double *res, double src1){
	return cosh(src1);
}

//unimplemented
int Math_Sinh(double *res, double src1){
	return sinh(src1);
}

//unimplemented
int Math_Tanh(double *res, double src1){
	return tanh(src1);
}

//unimplemented
int Math_Ln(double *res, double src1){
	return log(src1);
}

int Math_Log10(double *res, double src1){
	if(src1 <= 0) return 1;
	
	*res = log10(src1);
	return 0;
}

//unimplemented
int Math_Modf(double *res, double src1, double *integer){ //Returns fractional part, sets integer to int part
	return modf(src1, integer);
}

int Math_Pow(double *res, double x, double y){ // x^y
	*res = pow(x, y);
	return 0;
}

int Math_Sqrt(double *res, double src1){
	if(src1 < 0) return 1;
	
	*res = sqrt(src1);
	return 0;
}

int Math_Abs(double *res, double src1){
	*res = fabs(src1);
	return 0;
}

int Math_Neg(double *res, double src1){
	*res = -src1;
	return 0;
}
