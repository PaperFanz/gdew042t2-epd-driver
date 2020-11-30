// Main.c
// Runs on TM4C123
// The Squad
// November 28, 2020

#include <stdint.h>
#include <stdio.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/CortexM.h"
#include "AlphaValues.h"

double AlphaValuesArr[26];

int key_to_idx(key_t key){
	switch(key){
		case A:
			return 0;
		case B:
			return 1;
		case C:
			return 2;
		case D:
			return 3;
		case E:
			return 4;
		case F:
			return 5;
		case G:
			return 6;
		case H:
			return 7;
		case I:
			return 8;
		case J:
			return 9;
		case K:
			return 10;
		case L:
			return 11;
		case M:
			return 12;
		case N:
			return 13;
		case O:
			return 14;
		case P:
			return 15;
		case Q:
			return 16;
		case R:
			return 17;
		case S:
			return 18;
		case T:
			return 19;
		case U:
			return 20;
		case V:
			return 21;
		case W:
			return 22;
		case X:
			return 23;
		case Y:
			return 24;
		case Z:
			return 25;
		default:
			return 0; //error will give A
	}
}

int node_to_idx(node_t node){
	switch(node){
		case VA:
			return 0;
		case VB:
			return 1;
		case VC:
			return 2;
		case VD:
			return 3;
		case VE:
			return 4;
		case VF:
			return 5;
		case VG:
			return 6;
		case VH:
			return 7;
		case VI:
			return 8;
		case VJ:
			return 9;
		case VK:
			return 10;
		case VL:
			return 11;
		case VM:
			return 12;
		case VN:
			return 13;
		case VO:
			return 14;
		case VP:
			return 15;
		case VQ:
			return 16;
		case VR:
			return 17;
		case VS:
			return 18;
		case VT:
			return 19;
		case VU:
			return 20;
		case VV:
			return 21;
		case VW:
			return 22;
		case VX:
			return 23;
		case VY:
			return 24;
		case VZ:
			return 25;
		default:
			return 0; //error will give A
	}
}

double get_key_alpha_value(key_t key){
	return AlphaValuesArr[key_to_idx(key)];
}

void set_key_alpha_value(key_t key, double val){
	AlphaValuesArr[key_to_idx(key)] = val;
}

double get_node_alpha_value(node_t node){
	return AlphaValuesArr[node_to_idx(node)];
}

void set_node_alpha_value(node_t node, double val){
	AlphaValuesArr[node_to_idx(node)] = val;
}
