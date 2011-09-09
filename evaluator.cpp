#include <iostream>

#include <error.h>
#include <assert.h>

#include "evaluator.h"
#include "environment.h"
#include "primitive.h"
#include "function.h"
#include "reader.h"
#include "special.h"
#include "symbol.h"
#include "pair.h"
#include "printer.h"

context_t::context_t(value_t environment) {
	this->environment = environment;

	this->accumulator = UNSPECIFIED;
	this->value_stack = EMPTY_LIST;
	this->value_stack_size = 0;
	this->frame_stack = EMPTY_LIST;
}

static inline
void evaluate_op_test(context_t* context, value_t args) {
	value_t consequence = pair_left(args);
	value_t alternative = pair_left(pair_right(args));
	if (context->accumulator != BOOLEAN_FALSE) {
		context->next_expr = consequence;
	}
	else {
		context->next_expr = alternative;
	}
}

#include "fixnum.h"
static inline
void evaluate_op_return(context_t* context, value_t args) {
	value_t frame = pair_left(context->frame_stack);
	context->frame_stack = pair_right(context->frame_stack);

	context->environment = pair_left(frame);
	context->value_stack = pair_left(pair_right(frame));
	context->value_stack_size = unwrap_fixnum(pair_left(pair_right(pair_right(frame))));
	context->next_expr = pair_left(pair_right(pair_right(pair_right(frame))));
}
static inline
void evaluate_op_frame(context_t* context, value_t args) {
	value_t ret = pair_left(args);
	value_t next = pair_left(pair_right(args));

	value_t frame = make_list(context->environment, context->value_stack,
	                                                wrap_fixnum(context->value_stack_size),
	                                                ret, 0);
	context->value_stack = EMPTY_LIST;
	context->value_stack_size = 0;
	context->frame_stack = make_pair(frame, context->frame_stack);
	context->next_expr = next;
}
static inline
void evaluate_op_constant(context_t* context, value_t args) {
	value_t constant = pair_left(args);
	value_t next = pair_left(pair_right(args));

	context->next_expr = next;
	context->accumulator = constant;
}

static inline
void evaluate_function_application(context_t* context, 
	                               value_t func, 
	                               value_t args) {
	value_t params = function_get_params(func);
	value_t env = make_environment(function_get_environment(func),
								   params,
								   args);
	value_t body = function_get_body(func);

	context->environment = env;
	context->value_stack = EMPTY_LIST;
	context->value_stack_size = 0;
	context->next_expr = body;
}

static inline
void evaluate_op_apply(context_t* context, value_t args) {
	value_t func = context->accumulator;
	// FIXME: Ugly hack to make the 'apply' procedure easy to implement. Gotta
	// check this out later.
	if (is_primitive(func)) {
		primitive_apply(context, func);
	}
	else if (is_function(func)) {
		value_t arg_list = context->value_stack;
		evaluate_function_application(context, func, arg_list);
	}
	else {
		error(1, 0, "Trying to apply something that is neither a function nor a primitive.");
	}
}

static inline
void evaluate_op_argument(context_t* context, value_t args) {
	value_t next = pair_left(args);
	// Push the current accumulator to value stack.
	context->value_stack = make_pair(context->accumulator, context->value_stack);
	context->value_stack_size++;
	context->next_expr = next;
}

static inline
void evaluate_op_lookup(context_t* context, value_t args) {
	value_t variable = pair_left(args);
	value_t next = pair_left(pair_right(args));

	context->next_expr = next;
	context->accumulator = environment_get(context->environment, variable);
}

static inline
void evaluate_op_closure(context_t* context, value_t args) {
	value_t params = pair_left(args);
	value_t body = pair_left(pair_right(args));
	value_t next = pair_left(pair_right(pair_right(args)));

	value_t env = context->environment;

	context->accumulator = make_function(env, params, body);
	context->next_expr = next;
}

static inline
void evaluate_op_bind(context_t* context, value_t args) {
	value_t name = pair_left(args);
	value_t next = pair_left(pair_right(args));

	environment_add(context->environment, name, context->accumulator);
	context->accumulator = UNSPECIFIED;
	context->next_expr = next;
}
value_t evaluate(context_t* context) {
	value_t op_code = pair_left(context->next_expr);
	value_t args = pair_right(context->next_expr);
	
	while (op_code != OP_HALT) {
		if (op_code == OP_CONSTANT) {
			evaluate_op_constant(context, args);
		}
		else if (op_code == OP_LOOKUP) {
			evaluate_op_lookup(context, args);
		}
		else if (op_code == OP_CLOSURE) {
			evaluate_op_closure(context, args);
		}
		else if (op_code == OP_TEST) {
			evaluate_op_test(context, args);
		}
		else if (op_code == OP_ASSIGN) {
			error(1, 0, "Not yet implemented.");
		}
		else if (op_code == OP_SAVE) {
			error(1, 0, "Not yet implemented.");
		}
		else if (op_code == OP_REIFY) {
			error(1, 0, "Not yet implemented.");
		}
		else if (op_code == OP_FRAME) {
			evaluate_op_frame(context, args);
		}
		else if (op_code == OP_APPLY) {
			evaluate_op_apply(context, args);
		}
		else if (op_code == OP_ARGUMENT) {
			evaluate_op_argument(context, args);
		}
		else if (op_code == OP_RETURN) {
			evaluate_op_return(context, args);
		}
		else if (op_code == OP_BIND) {
			evaluate_op_bind(context, args);
		}

		op_code = pair_left(context->next_expr);
		args = pair_right(context->next_expr);
	}
	return context->accumulator;
}

