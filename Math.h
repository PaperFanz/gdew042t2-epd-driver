#ifndef _MATH_H_
#define _MATH_H_

//Functions

double Math_Add(double src1, double src2);

double Math_Sub(double src1, double src2);

double Math_Mul(double src1, double src2);

double Math_Div(double src1, double src2);

double Math_Sin(double src1); //All Trig functions work in radians

double Math_Cos(double src1);

double Math_Tan(double src1);

double Math_Cot(double src1);

double Math_Csc(double src1);

double Math_Sec(double src1);

double Math_ExponentE(double src1); //e^x

double Math_ArcSin(double src1);

double Math_ArcCos(double src1);

double Math_ArcTan(double src1);

double Math_ArcCot(double src1); //These next 3 might be wrong

double Math_ArcCsc(double src1);

double Math_ArcSec(double src1);

double Math_Cosh(double src1);

double Math_Sinh(double src1);

double Math_Tanh(double src1);

double Math_Ln(double src1);

double Math_Log10(double src1);

double Math_Modf(double src1, double *integer); //Retruns fractional part, sets integer to int part

double Math_Pow(double x, double y); // x^y

double Math_Sqrt(double src1);

double Math_Abs(double src1);

double Math_Neg(double src1);

#endif
