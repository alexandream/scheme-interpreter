#include <error.h>
#include <string>
#include <sstream>

#include "value.h"
#include "builtin_primitives.h"
#include "garbage_collector.h"
#include "compiler.h"
#include "primitive.h"
#include "fixnum.h"
#include "symbol.h"
#include "special.h"
#include "pair.h"


static uint64_t gensym_count = 0;

static
void BUILTIN_RETURN(context_t* context, value_t value) {
	context->accumulator = value;
	context->value_stack = EMPTY_LIST;
	context->value_stack_size = 0;
	context->next_expr = make_list(OP_RETURN, 0);
}

// BOOLEAN PRIMITIVES 
//
void BP_not(context_t* context) {
    value_t param = pair_left(context->value_stack);
	BUILTIN_RETURN(context, (param == BOOLEAN_FALSE)?
                                         BOOLEAN_TRUE
                                       : BOOLEAN_FALSE);
}

void BP_and(context_t* context) {
	value_t op1 = pair_left(context->value_stack);
	value_t op2 = pair_left(pair_right(context->value_stack));

	BUILTIN_RETURN(context, wrap_boolean( unwrap_boolean(op1) && unwrap_boolean(op2) ));
}

void BP_or(context_t* context) {
	value_t op1 = pair_left(context->value_stack);
	value_t op2 = pair_left(pair_right(context->value_stack));

	BUILTIN_RETURN(context, wrap_boolean( unwrap_boolean(op1) || unwrap_boolean(op2) ));
}

// EQUIVALENCE PRIMITIVES
//
void BP_eqP(context_t* context) {
	value_t op1 = pair_left(context->value_stack);
	value_t op2 = pair_left(pair_right(context->value_stack));

	BUILTIN_RETURN(context, wrap_boolean(op1 == op2));
}
// PAIR PRIMITIVES
//
void BP_cons(context_t* context) {
	value_t op1 = pair_left(context->value_stack);
	value_t op2 = pair_left(pair_right(context->value_stack));

	BUILTIN_RETURN(context, make_pair(op1, op2));
}

void BP_car(context_t* context) {
	value_t oper = pair_left(context->value_stack);
	BUILTIN_RETURN(context, pair_left(oper));
}

void BP_cdr(context_t* context) {
	value_t oper = pair_left(context->value_stack);

	BUILTIN_RETURN(context, pair_right(oper));
}

void BP_list(context_t* context) {
	BUILTIN_RETURN(context, context->value_stack);
}
// CONTROL PRIMITIVES
//
void BP_apply(context_t* context) {
	value_t func = pair_left(context->value_stack);
	value_t params = pair_right(context->value_stack);
	value_t expanded_params = (params == EMPTY_LIST)? EMPTY_LIST:
                                                      pair_left(params);
	context->value_stack = expanded_params;
	context->accumulator = func;
	context->next_expr = make_list(OP_APPLY, 0);

}
// TYPE PREDICATE PRIMITIVES
//
void BP_symbolP(context_t* context) {
	value_t param = pair_left(context->value_stack);
	BUILTIN_RETURN(context, wrap_boolean(is_symbol(param)));
}

void BP_pairP(context_t* context) {
	value_t param = pair_left(context->value_stack);
    value_t result = wrap_boolean(is_pair(param));

	BUILTIN_RETURN(context, result);

}

void BP_unspecifiedP(context_t* context) {
    value_t param = pair_left(context->value_stack);
    BUILTIN_RETURN(context, wrap_boolean(param == UNSPECIFIED));
}
// FIXNUM PRIMITIVES
//
void BP_fixnum_PLUS(context_t* context) {
	int64_t result = 0;
	int32_t nargs = context->value_stack_size;
	value_t args = context->value_stack;
	for (int i = 0; i < nargs; i++) {
		result += unwrap_fixnum(pair_left(args));
		args = pair_right(args);
	}
	BUILTIN_RETURN(context, wrap_fixnum(result));
}
void BP_fixnum_MINUS(context_t* context) {
	int64_t result = 0;
	int32_t nargs = context->value_stack_size;
	value_t args = context->value_stack;
	if (nargs > 0) {
		result = unwrap_fixnum(pair_left(args));
		args = pair_right(args);
		nargs--;
	}
	for (int i = 0; i < nargs; i++) {
		result -= unwrap_fixnum(pair_left(args));
		args = pair_right(args);
	}
	BUILTIN_RETURN(context, wrap_fixnum(result));
}
void BP_fixnum_LTHAN(context_t* context) {
	value_t args = context->value_stack;
	int32_t arg1 = unwrap_fixnum(pair_left(args));
	int32_t arg2 = unwrap_fixnum(pair_left(pair_right(args)));

	BUILTIN_RETURN(context, wrap_boolean(arg1 < arg2));
}
// TEST PRIMITIVES
//
void BP_max_fixnum(context_t* context) {
	BUILTIN_RETURN(context, FIXNUM_MAX);
}

void BP_min_fixnum(context_t* context) {
	BUILTIN_RETURN(context, FIXNUM_MIN);
}

void BP_gensym(context_t* context) {
	std::ostringstream sstream;
	sstream << gensym_count++;
	std::string symbol_string = sstream.str();

	BUILTIN_RETURN(context, make_symbol(symbol_string + "-gensym"));
}


void BP_collect(context_t* context) {
	collect();
	BUILTIN_RETURN(context, UNSPECIFIED);
}
