#include <error.h>
#include <string>
#include <stdio.h>
#include <vector>
#include <sstream>
#include <math.h>

#include "value.h"
#include "builtin_primitives.h"
#include "garbage_collector.h"
#include "macro.h"
#include "compiler.h"
#include "primitive.h"
#include "function.h"
#include "fixnum.h"
#include "string.h"
#include "printer.h"
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

void BP_set_carB(context_t* context) {
	value_t pair = pair_left(context->value_stack);
	value_t value = pair_left(pair_right(context->value_stack));
	pair_set_left(pair, value);
	BUILTIN_RETURN(context, UNSPECIFIED);
}
void BP_set_cdrB(context_t* context) {
	value_t pair = pair_left(context->value_stack);
	value_t value = pair_left(pair_right(context->value_stack));
	pair_set_right(pair, value);
	BUILTIN_RETURN(context, UNSPECIFIED);
}
void BP_list(context_t* context) {
	BUILTIN_RETURN(context, context->value_stack);
}
// CONTROL PRIMITIVES

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
void BP_booleanP(context_t* context) {
	value_t param = pair_left(context->value_stack);
	value_t result = wrap_boolean(is_boolean(param));
	BUILTIN_RETURN(context, result);
}
void BP_unspecifiedP(context_t* context) {
    value_t param = pair_left(context->value_stack);
    BUILTIN_RETURN(context, wrap_boolean(param == UNSPECIFIED));
}
void BP_fixnumP(context_t* context) {
	value_t param = pair_left(context->value_stack);
	BUILTIN_RETURN(context, wrap_boolean(is_fixnum(param)));
}
void BP_procedureP(context_t* context) {
	value_t param = pair_left(context->value_stack);
	bool result = (is_primitive(param) || is_function(param));
	BUILTIN_RETURN(context, wrap_boolean(result));
}
void BP_charP(context_t* context) {
	value_t param = pair_left(context->value_stack);
	bool result = (is_character(param));
	BUILTIN_RETURN(context, wrap_boolean(result));
}
// FIXNUM PRIMITIVES
//
static int64_t BINOP_fixnum_add(int64_t op1, int64_t op2) { return op1 + op2; }
static int64_t BINOP_fixnum_MUL(int64_t op1, int64_t op2) { return op1 * op2; }
static int64_t BINOP_fixnum_DIV(int64_t op1, int64_t op2) { return op1 / op2; }
static int64_t BINOP_fixnum_MOD(int64_t op1, int64_t op2) { return op1 % op2; }

static int64_t BINOP_fixnum_FLOOR_DIV(int64_t op1, int64_t op2) {
    return floor(((double) op1) / op2 );
}
static int64_t BINOP_fixnum_FLOOR_REM(int64_t op1, int64_t op2) {
    return (op1 - BINOP_fixnum_FLOOR_DIV(op1, op2) * op2);
}
static int64_t BINOP_fixnum_CEIL_DIV(int64_t op1, int64_t op2) {
    return ceil( ((double) op1) / op2 );
}
static int64_t BINOP_fixnum_CEIL_REM(int64_t op1, int64_t op2) {
    return (op1 - BINOP_fixnum_CEIL_DIV(op1, op2) * op2);
}
static int64_t BINOP_fixnum_ROUND_DIV(int64_t op1, int64_t op2) {
	double quotient = ((double) op1) / op2;
	int64_t rounded = (quotient > 0.0) ? floor(quotient + 0.5)
	                                   : ceil(quotient - 0.5);
	return rounded;
}
static int64_t BINOP_fixnum_ROUND_REM(int64_t op1, int64_t op2) {
	return (op1 - BINOP_fixnum_ROUND_DIV(op1, op2) * op2);
}
static int64_t BINOP_fixnum_EUCLID_DIV(int64_t op1, int64_t op2) {
	return (op2 > 0) ? BINOP_fixnum_FLOOR_DIV(op1, op2)
	                 : BINOP_fixnum_CEIL_DIV(op1, op2);
}
static int64_t BINOP_fixnum_EUCLID_REM(int64_t op1, int64_t op2) {
	return (op1 - BINOP_fixnum_EUCLID_DIV(op1, op2) * op2);
}

typedef int64_t(*fixnum_binop)(int64_t, int64_t);

static inline
int64_t AUX_fixnum_repeated_op(value_t args,
                               int32_t nargs,
							   int64_t initial,
                               fixnum_binop func) {
	int64_t result = initial;
	for (int i = 0; i < nargs; i++) {
         int64_t operand = unwrap_fixnum(pair_left(args));
         result = func(result, operand);
         args = pair_right(args);
	}
	return result;
}
	
void BP_fixnum_PLUS(context_t* context) {
	int64_t result = AUX_fixnum_repeated_op(context->value_stack,
                                            context->value_stack_size,
											0,
											BINOP_fixnum_add);
	BUILTIN_RETURN(context, wrap_fixnum(result));
}

void BP_fixnum_MUL(context_t* context) {
	int64_t result = AUX_fixnum_repeated_op(context->value_stack,
                                            context->value_stack_size,
                                            1,
											BINOP_fixnum_MUL);
	BUILTIN_RETURN(context, wrap_fixnum(result));
}

void BP_fixnum_DIV(context_t* context) {
	int32_t nargs = context->value_stack_size;
	value_t args = context->value_stack;
	int64_t result = 0;
	int64_t initial = unwrap_fixnum(pair_left(args));
	if (nargs > 1) {
		int64_t rest = AUX_fixnum_repeated_op(pair_right(args),
		                                      nargs - 1,
		                                      1,
		                                      BINOP_fixnum_MUL);
		result = initial / rest;
	}
	else {
		result = 1 / initial;
	}
	BUILTIN_RETURN(context, wrap_fixnum(result));
}
void BP_fixnum_MINUS(context_t* context) {
	int32_t nargs = context->value_stack_size;
	value_t args = context->value_stack;
	int64_t result;
	int64_t initial = unwrap_fixnum(pair_left(args));
	if (nargs > 1) {
		int64_t rest = AUX_fixnum_repeated_op(pair_right(args),
	                                          nargs - 1,
	                                          0,
	                                          BINOP_fixnum_add);
		result = initial - rest;
	}
	else {
		result = -initial;
	}
	BUILTIN_RETURN(context, wrap_fixnum(result));
}
void BP_fixnum_apply_binop(context_t* context, fixnum_binop func) {
	value_t args = context->value_stack;
	int64_t op1 = unwrap_fixnum(pair_left(args));
	int64_t op2 = unwrap_fixnum(pair_left(pair_right(args)));
	
	int64_t result = func(op1, op2);
	BUILTIN_RETURN(context, wrap_fixnum(result));
}

void BP_fixnum_truncate_quotient(context_t* context) {
	BP_fixnum_apply_binop(context, BINOP_fixnum_DIV);
}
void BP_fixnum_truncate_remainder(context_t* context) {
	BP_fixnum_apply_binop(context, BINOP_fixnum_MOD);
}
void BP_fixnum_floor_quotient(context_t* context) {
	BP_fixnum_apply_binop(context, BINOP_fixnum_FLOOR_DIV);
}
void BP_fixnum_floor_remainder(context_t* context) {
	BP_fixnum_apply_binop(context, BINOP_fixnum_FLOOR_REM);
}
void BP_fixnum_ceil_quotient(context_t* context) {
	BP_fixnum_apply_binop(context, BINOP_fixnum_CEIL_DIV);
}
void BP_fixnum_ceil_remainder(context_t* context) {
	BP_fixnum_apply_binop(context, BINOP_fixnum_CEIL_REM);
}
void BP_fixnum_round_quotient(context_t* context) {
	BP_fixnum_apply_binop(context, BINOP_fixnum_ROUND_DIV);
}
void BP_fixnum_round_remainder(context_t* context) {
	BP_fixnum_apply_binop(context, BINOP_fixnum_ROUND_REM);
}
void BP_fixnum_euclid_quotient(context_t* context) {
	BP_fixnum_apply_binop(context, BINOP_fixnum_EUCLID_DIV);
}
void BP_fixnum_euclid_remainder(context_t* context) {
	BP_fixnum_apply_binop(context, BINOP_fixnum_EUCLID_REM);
}
enum fixnum_relop {
	LESS_THAN,
	GREATER_THAN,
	EQUAL,
	LESS_THAN_EQUAL,
	GREATER_THAN_EQUAL
};

static inline
bool AUX_fixnum_relational(value_t op1, value_t op2, fixnum_relop oper) {
	int64_t fixnum_op1 = unwrap_fixnum(op1);
	int64_t fixnum_op2 = unwrap_fixnum(op2);
	bool result = false;
	switch (oper) {
		case LESS_THAN:
			result = fixnum_op1 < fixnum_op2; break;
		case GREATER_THAN:
			result = fixnum_op1 > fixnum_op2; break;
		case LESS_THAN_EQUAL:
			result = fixnum_op1 <= fixnum_op2; break;
		case GREATER_THAN_EQUAL:
			result = fixnum_op1 >= fixnum_op2; break;
		case EQUAL:
			result = fixnum_op1 == fixnum_op2; break;
	}
	return result;
}
static inline
bool BP_fixnum_relational(value_t args, fixnum_relop oper) {
	if (!is_pair(args))
		return true;
	
	value_t first = pair_left(args);
	args = pair_right(args);
	
	if (!is_pair(args))
		return true;
	
	value_t second = pair_left(args);
	args = pair_right(args);
    
	bool result = false;
	if (AUX_fixnum_relational(first, second, oper)) {
		result = true;
		while (is_pair(args)) {
			first = second;
			second = pair_left(args);
			if (!AUX_fixnum_relational(first, second, oper)) {
				result = false;
				break;
			}
			args = pair_right(args);
		}
    }
	return result;
}

void BP_fixnum_LT(context_t* context) {
	bool result = BP_fixnum_relational(context->value_stack, LESS_THAN);
	BUILTIN_RETURN(context, wrap_boolean(result));
}
void BP_fixnum_GT(context_t* context) {
	bool result = BP_fixnum_relational(context->value_stack, GREATER_THAN);
	BUILTIN_RETURN(context, wrap_boolean(result));
}
void BP_fixnum_LTE(context_t* context) {
	bool result = BP_fixnum_relational(context->value_stack, LESS_THAN_EQUAL);
	BUILTIN_RETURN(context, wrap_boolean(result));
}
void BP_fixnum_GTE(context_t* context) {
	bool result = BP_fixnum_relational(context->value_stack, GREATER_THAN_EQUAL);
	BUILTIN_RETURN(context, wrap_boolean(result));
}
void BP_fixnum_EQ(context_t* context) {
	bool result = BP_fixnum_relational(context->value_stack, EQUAL);
	BUILTIN_RETURN(context, wrap_boolean(result));
}

static inline char convert_radix(int64_t int_radix) {
	switch (int_radix) {
		case 2:
			return 'b';
		case 8:
			return 'o';
		case 10:
			return 'd';
		case 16:
			return 'x';
		default:
			error(1, 0, "Invalid radix.");
			return ' ';
	}	
}
void BP_fixnum_to_string(context_t* context) {
	value_t param = pair_left(context->value_stack);
	char radix = 'd';
	if (context->value_stack_size > 1) {
		value_t radix_param = pair_left(pair_right(context->value_stack));
		int64_t int_radix = unwrap_fixnum(radix_param);
		radix = convert_radix(int_radix);
	}
	std::string result = fixnum_format(param, radix);
	BUILTIN_RETURN(context, wrap_string(result));
}
static
bool is_hex_digit(char d) {
	return (  (d >= '0' && d <= '9') 
	       || (d >= 'a' && d <= 'f')
	       || (d >= 'A' && d <= 'F'));
}
static
bool is_dec_digit(char d) {
	return (d >= '0' && d <= '9');
}
static
bool is_bin_digit(char d) {
	return (d >= '0' && d <= '1');
}
static
bool is_oct_digit(char d) {
	return (d >= '0' && d <= '7');
}
void BP_fixnum_from_string(context_t* context) {
	value_t param = pair_left(context->value_stack);
	char radix = 'd';
	if (context->value_stack_size > 1) {
		value_t radix_param = pair_left(pair_right(context->value_stack));
		int64_t int_radix = unwrap_fixnum(radix_param);
		radix = convert_radix(int_radix);
	}
	std::string str = unwrap_string(param);
	bool (*isdigitP)(char) = NULL;
	isdigitP = (radix == 'd') ? is_dec_digit 
	         : (radix == 'x') ? is_hex_digit
	         : (radix == 'b') ? is_bin_digit
	         : (radix == 'o') ? is_oct_digit
	         : NULL;
	value_t result = UNSPECIFIED;
	for (int i = 0; i < str.length(); i++) {
		if (!isdigitP(str[i])) {
			result = BOOLEAN_FALSE;
			break;
		}
	}
	if (result == UNSPECIFIED) {
		std::string radix_str = "#d";
		radix_str[1] = radix;
		
		result = fixnum_preprocess(radix_str + str);
	}
	BUILTIN_RETURN(context, result);
}

// SYMBOL PRIMITIVES
//
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

