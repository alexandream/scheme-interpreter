#include <string>
#include <sstream>
#include <vector>

#include "pair.h"
#include "value.h"
#include "macro.h"
#include "string.h"
#include "symbol.h"
#include "printer.h"
#include "function.h"
#include "evaluator.h"
#include "primitive.h"
#include "garbage_collector.h"

static uint64_t gensym_count = 0;

void BP_gensym(context_t* context) {
	std::ostringstream sstream;
	sstream << gensym_count++;
	std::string symbol_string = sstream.str();

	BUILTIN_RETURN(context, make_symbol(symbol_string + "-gensym"));
}


// BOOLEAN PRIMITIVES 
//
void BP_booleanP(context_t* context) {
	value_t param = pair_left(context->value_stack);
	value_t result = wrap_boolean(is_boolean(param));
	BUILTIN_RETURN(context, result);
}

void BP_not(context_t* context) {
    value_t param = pair_left(context->value_stack);
	BUILTIN_RETURN(context, (param == BOOLEAN_FALSE)?
                                         BOOLEAN_TRUE
                                       : BOOLEAN_FALSE);
}


// MISC TYPE PREDICATES
//
void BP_unspecifiedP(context_t* context) {
    value_t param = pair_left(context->value_stack);
    BUILTIN_RETURN(context, wrap_boolean(param == UNSPECIFIED));
}
void BP_procedureP(context_t* context) {
	value_t param = pair_left(context->value_stack);
	bool result = (is_primitive(param) || is_function(param));
	BUILTIN_RETURN(context, wrap_boolean(result));
}


// EQUIVALENCE PREDICATES
//
void BP_eqP(context_t* context) {
	value_t op1 = pair_left(context->value_stack);
	value_t op2 = pair_left(pair_right(context->value_stack));

	BUILTIN_RETURN(context, wrap_boolean(op1 == op2));
}


// SYMBOL PRIMITIVES
//
void BP_symbolP(context_t* context) {
	value_t param = pair_left(context->value_stack);
	BUILTIN_RETURN(context, wrap_boolean(is_symbol(param)));
}

void BP_symbol_to_string(context_t* context) {
	value_t param = pair_left(context->value_stack);
	std::string str_symbol = symbol_format(param);
	BUILTIN_RETURN(context, wrap_string(str_symbol));
}

void BP_symbol_from_string(context_t* context) {
	value_t param = pair_left(context->value_stack);
	std::string str_symbol = unwrap_string(param);
	BUILTIN_RETURN(context, make_symbol(str_symbol));
}


// CONTROL PRIMITIVES
//
void BP_apply(context_t* context) {
	value_t func = pair_left(context->value_stack);
	value_t params = pair_right(context->value_stack);
	int32_t nargs = context->value_stack_size;
	std::vector<value_t> args(nargs);
	for(int i = 0; i < nargs - 2; i++) {
		args[i] = pair_left(params);
		params = pair_right(params);
	}
	params = pair_left(params);
	
	for(int i = nargs - 3; i >= 0; i--) {
		params = make_pair(args[i], params);
	}
	
	context->value_stack = params;
	context->value_stack_size = pair_linked_length(params);
	context->accumulator = func;
	context->next_expr = make_list(OP_APPLY, 0);
}

void BP_macro_expand(context_t* context) {
    value_t arg = pair_left(context->value_stack);
    BUILTIN_RETURN(context, macro_expand(arg));
}

void BP_collect(context_t* context) {
	collect();
	BUILTIN_RETURN(context, UNSPECIFIED);
}


void BP_echo(context_t* context) {
	value_t arg = pair_left(context->value_stack);
	println(arg);
	BUILTIN_RETURN(context, UNSPECIFIED);
}
