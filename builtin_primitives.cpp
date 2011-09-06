#include "builtin_primitives.h"
#include "special.h"
#include "pair.h"

// BOOLEAN PRIMITIVES 
//
value_t BP_not(value_t params) {
	return wrap_boolean(!unwrap_boolean(pair_left(params)));
}

value_t BP_and(value_t params) {
	value_t op1 = pair_left(params);
	value_t op2 = pair_left(pair_right(params));

	return wrap_boolean( unwrap_boolean(op1) && unwrap_boolean(op2) );
}

value_t BP_or(value_t params) {
	value_t op1 = pair_left(params);
	value_t op2 = pair_left(pair_right(params));

	return wrap_boolean( unwrap_boolean(op1) || unwrap_boolean(op2) );
}

// EQUIVALENCE PRIMITIVES
//
value_t BP_eqP(value_t params) {
	value_t op1 = pair_left(params);
	value_t op2 = pair_left(pair_right(params));

	return wrap_boolean(op1 == op2);
}
// PAIR PRIMITIVES
//
value_t BP_cons(value_t params) {
	value_t op1 = pair_left(params);
	value_t op2 = pair_left(pair_right(params));

	return make_pair(op1, op2);
}

value_t BP_car(value_t params) {
	value_t oper = pair_left(params);
	
	return pair_left(oper);
}

value_t BP_cdr(value_t params) {
	value_t oper = pair_left(params);

	return pair_right(oper);
}
