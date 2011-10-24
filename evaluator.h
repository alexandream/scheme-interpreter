#ifndef __EVALUATOR_H__
#define __EVALUATOR_H__
#include <list>

#include "value.h"


struct context_t {
	value_t accumulator;
	value_t next_expr;
	value_t environment;
	
	value_t value_stack;
	int32_t value_stack_size;

	value_t frame_stack;
	context_t(value_t environment);
	void show(void);
};

typedef std::list<context_t*> context_list_t;

context_t* make_context(value_t environment);
void dispose_context(context_t* context);
context_list_t* list_active_contexts(void);
value_t evaluate(context_t*);

#endif
