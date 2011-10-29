
#include "value.h"
#include "evaluator.h"
#include "pair.h"

void BP_pairP(context_t* context) {
	value_t param = pair_left(context->value_stack);
    value_t result = wrap_boolean(is_pair(param));

	BUILTIN_RETURN(context, result);
}

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
