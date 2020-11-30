#ifndef _ALPHA_VALUES_H_
#define _ALPHA_VALUES_H_

#include "keymap.h"
#include "ExpressionTree.h"

double get_key_alpha_value(key_t key);

double get_node_alpha_value(node_t node);

void set_key_alpha_value(key_t key, double val);

void set_node_alpha_value(node_t key, double val);

#endif
