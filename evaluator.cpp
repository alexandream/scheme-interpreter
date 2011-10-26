#include <list>

#include <error.h>
#include <assert.h>
#include <stdio.h>

#include "printer.h"

#include "evaluator.h"
#include "environment.h"
#include "primitive.h"
#include "function.h"
#include "formatter.h"
#include "reader.h"
#include "special.h"
#include "symbol.h"
#include "pair.h"
#include "macro.h"


static context_list_t context_list;

context_t::context_t(value_t environment) {
	this->environment = environment;

	this->accumulator = UNSPECIFIED;
	this->value_stack = EMPTY_LIST;
	this->value_stack_size = 0;
	this->frame_stack = EMPTY_LIST;
}

void context_t::show(void) {
	println(this->environment, "Environment: ");
	println(this->next_expr,   "Next  Expr.: ");
	println(this->accumulator, "Accumulator: ");
	println(this->value_stack, "Value Stack: ");
	println(this->frame_stack, "Frame Stack: ");
}

context_t* make_context(value_t environment) {
	context_t* result = new context_t(environment);
	context_list.push_back(result);
	return result;
}

context_list_t* list_active_contexts(void) {
	return &context_list;
}

void dispose_context(context_t* context) {
	context_list.remove(context);
	delete context;
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
	protect_value(frame);
		
	context->value_stack = EMPTY_LIST;
	context->value_stack_size = 0;
	context->frame_stack = make_pair(frame, context->frame_stack);
	context->next_expr = next;
	unprotect_storage(1);
}
static inline
void evaluate_op_constant(context_t* context, value_t args) {
	value_t constant = pair_left(args);
	value_t next = pair_left(pair_right(args));

	context->next_expr = next;
	context->accumulator = constant;
}

static inline
void evaluate_op_assign(context_t* context, value_t args) {
	value_t variable = pair_left(args);
	value_t next = pair_left(pair_right(args));
	
	environment_set(context->environment, variable, context->accumulator);

	context->next_expr = next;
	context->accumulator = UNSPECIFIED;
}
static inline
void evaluate_function_application(context_t* context, 
	                               value_t func, 
	                               value_t args) {
	value_t params = function_get_params(func);
	value_t env = make_environment(function_get_environment(func),
								   params,
								   args);
	protect_value(env);
	value_t body = function_get_body(func);

	context->environment = env;
	context->value_stack = EMPTY_LIST;
	context->value_stack_size = 0;
	context->next_expr = body;

	unprotect_storage(1);
}

static inline
void evaluate_op_apply(context_t* context, value_t args) {
	value_t func = context->accumulator;
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

static inline
value_t make_continuation(value_t stack) {
	value_t var = make_symbol("var");
	value_t body = make_list(OP_REIFY, stack, var, 0);
	protect_value(body);

	value_t env = UNSPECIFIED;
	value_t arg_list = make_list(var, 0);
	protect_value(arg_list);

	value_t continuation = make_function(env, arg_list, body);
	unprotect_storage(2);

	return continuation;
}

static inline
void evaluate_op_save(context_t* context, value_t args) {
	value_t next = pair_left(args);
	
	context->accumulator = make_continuation(context->frame_stack);

	context->next_expr = next;
}

static inline
void evaluate_op_reify(context_t* context, value_t args) {
	value_t stack = pair_left(args);
	value_t var = pair_left(pair_right(args));

	context->accumulator = environment_get(context->environment, var);
	context->next_expr = make_list(OP_RETURN, 0);
	context->frame_stack = stack;
}

static inline
void evaluate_op_bind_macro(context_t* context, value_t args) {
	value_t name = pair_left(args);
	value_t next = pair_left(pair_right(args));

	macro_set_rewriter(name, context->accumulator);

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
			evaluate_op_assign(context, args);
		}
		else if (op_code == OP_SAVE) {
			evaluate_op_save(context, args);
		}
		else if (op_code == OP_REIFY) {
			evaluate_op_reify(context, args);
		}
		else if (op_code == OP_FRAME) {
            //printf("Evaluating OP_FRAME: %LX with next: %LX\n",
            //       context->next_expr,
            //       pair_left(args));    
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
		else if (op_code == OP_BIND_MACRO) {
			evaluate_op_bind_macro(context, args);
		}
		else {
			error(1, 0, "Unknown op code: %s", (format(op_code).c_str()));
		}
		op_code = pair_left(context->next_expr);
		args = pair_right(context->next_expr);
	}
	return context->accumulator;
}

