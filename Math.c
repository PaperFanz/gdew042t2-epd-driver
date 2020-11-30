// Math.c
// Runs on TM4C123
// The Squad
// October 23, 2020

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#include "Math.h"
#include <math.h>

double Math_Add(double src1, double src2){
	return src1 + src2;
}

double Math_Sub(double src1, double src2){
	return src1 - src2;
}

double Math_Mul(double src1, double src2){
	return src1 * src2;
}

double Math_Div(double src1, double src2){
	return src1 / src2;
}

double Math_Sin(double src1){
	return sin(src1);
}

double Math_Cos(double src1){
	return cos(src1);
}

double Math_Tan(double src1){
	return tan(src1);
}

double Math_Cot(double src1){
	return 1 / (tan(src1));
}

double Math_Csc(double src1){
	return 1/(sin(src1));
}

double Math_Sec(double src1){
	return 1 / (cos(src1));
}

double Math_ExponentE(double src1){ //e^x
	return exp(src1);
}

double Math_ArcSin(double src1){
	return asin(src1);
}

double Math_ArcCos(double src1){
	return acos(src1);
}

double Math_ArcTan(double src1){
	return atan(src1);
}

double Math_ArcCot(double src1){ //These next 3 might be wrong
	return 1 / (atan(src1));
}

double Math_ArcCsc(double src1){
	return 1/(asin(src1));
}

double Math_ArcSec(double src1){
	return 1 / (acos(src1));
}

double Math_Cosh(double src1){
	return cosh(src1);
}

double Math_Sinh(double src1){
	return sinh(src1);
}

double Math_Tanh(double src1){
	return tanh(src1);
}

double Math_Ln(double src1){
	return log(src1);
}

double Math_Log10(double src1){
	return log10(src1);
}

double Math_Modf(double src1, double *integer){ //Returns fractional part, sets integer to int part
	return modf(src1, integer);
}

double Math_Pow(double x, double y){ // x^y
	return pow(x, y);
}

double Math_Sqrt(double src1){
	return sqrt(src1);
}

double Math_Abs(double src1){
	return fabs(src1);
}

double Math_Neg(double src1){
	return -src1;
}
