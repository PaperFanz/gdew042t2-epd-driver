#ifndef _MATH_H_
#define _MATH_H_

#include <math.h>

#define CONST_PI acos(-1.0)
#define CONST_E exp(1)
#define CONST_PHI (1 + sqrt(5)) / 2  //Golden ratio
#define CONST_PLANCK 6.626068 * pow(10, -34) //m^2 * kg/s
#define CONST_AVOGADO 6.0221515 * pow(10, 23)
#define CONST_LIGHT 299792458 // m/s
#define CONST_GRAV 6.67300 * pow(10, -11) // m^3 kg^-1 s^-2
#define CONST_GRAV_ACCEL 9.80665 // m * s^-2
#define CONST_BOLT 1.380650 * pow(10, 23) // joule per kelvin

//Functions

int Math_Add(double *res, double src1, double src2);

int Math_Sub(double *res, double src1, double src2);

int Math_Mul(double *res, double src1, double src2);

int Math_Div(double *res, double src1, double src2);

int Math_Sin(double *res, double src1); //All Trig functions work in radians

int Math_Cos(double *res, double src1);

int Math_Tan(double *res, double src1);

int Math_Cot(double *res, double src1);

int Math_Csc(double *res, double src1);

int Math_Sec(double *res, double src1);

int Math_ExponentE(double *res, double src1); //e^x

int Math_ArcSin(double *res, double src1);

int Math_ArcCos(double *res, double src1);

int Math_ArcTan(double *res, double src1);

int Math_ArcCot(double *res, double src1); //These next 3 might be wrong

int Math_ArcCsc(double *res, double src1);

int Math_ArcSec(double *res, double src1);

int Math_Cosh(double *res, double src1);

int Math_Sinh(double *res, double src1);

int Math_Tanh(double *res, double src1);

int Math_Ln(double *res, double src1);

int Math_Log10(double *res, double src1);

int Math_Modf(double *res, double src1, double *integer); //Retruns fractional part, sets integer to int part

int Math_Pow(double *res, double x, double y); // x^y

int Math_Sqrt(double *res, double src1);

int Math_Abs(double *res, double src1);

int Math_Neg(double *res, double src1);

#endif
