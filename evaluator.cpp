#include <error.h>
#include <assert.h>

#include "evaluator.h"
#include "environment.h"
#include "primitive.h"
#include "reader.h"
#include "special.h"
#include "symbol.h"
#include "pair.h"


value_t evaluate_list(value_t expr, environment_t* env);
value_t evaluate_primitive_application(value_t expr, environment_t* env);

value_t QUOTE_SYMBOL = UNSPECIFIED;
value_t evaluate(value_t expr, environment_t *env) {
	// TODO: Take this initialization from here and put it in a place that
	// is run only once.
	if (QUOTE_SYMBOL == UNSPECIFIED) {
		QUOTE_SYMBOL = make_symbol("quote");
	}
	value_t result = UNSPECIFIED;
	
	if (is_boolean(expr)) {
		result = expr;
	}
	else if (is_symbol(expr)) {
		result = environment_get(env, expr);
	}
	else if (is_pair(expr)) {
		value_t head = pair_left(expr);
		if (head == QUOTE_SYMBOL) {
			// TODO: Handle case where there's no argument to quote.
			return pair_left(pair_right(expr));
		}
		else {
			result = evaluate_primitive_application(expr, env);
		}
	}
	else {
		error(1, 0, "Could not evaluate value 0x%016lX\n: Unknown type.", expr);
	}

	return result;
}

value_t evaluate_list(value_t expr, environment_t* env) {
	if (expr == EMPTY_LIST) {
		return EMPTY_LIST;
	}
	else {
		return make_pair(evaluate(pair_left(expr), env), evaluate_list(pair_right(expr), env));
	}
}

value_t evaluate_primitive_application(value_t expr, environment_t* env) {
	value_t func = evaluate(pair_left(expr), env);
	
	assert(is_primitive(func));

	value_t param_list = evaluate_list(pair_right(expr), env);
	return primitive_apply(func, param_list);
}

