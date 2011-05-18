
#include "boolean_primitives.h"
#include "special.h"
#include "pair.h"

value_t boolean_primitive_not(value_t params) {
	return wrap_boolean(!unwrap_boolean(pair_left(params)));
}

value_t boolean_primitive_and(value_t params) {
	value_t op1 = pair_left(params);
	value_t op2 = pair_left(pair_right(params));

	return wrap_boolean( unwrap_boolean(op1) && unwrap_boolean(op2) );
}

value_t boolean_primitive_or(value_t params) {
	value_t op1 = pair_left(params);
	value_t op2 = pair_left(pair_right(params));

	return wrap_boolean( unwrap_boolean(op1) || unwrap_boolean(op2) );
}
