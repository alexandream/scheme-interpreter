#ifndef __EVALUATOR_H__
#define __EVALUATOR_H__

#include "value.h"

struct context_t {
	value_t accumulator;
	value_t next_expr;
	value_t environment;
	value_t value_stack;
	value_t frame_stack;
};

value_t evaluate(context_t*);

#endif
