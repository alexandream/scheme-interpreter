#include <error.h>

#include "evaluator.h"
#include "environment.h"
#include "reader.h"
#include "special.h"
#include "symbol.h"
#include "cell.h"


value_t evaluate(value_t expr, environment_t *env) {
	value_t result = UNSPECIFIED;
	
	if (is_boolean(expr) || is_cons(expr)) {
		result = expr;
	}
	else if (is_symbol(expr)) {
		result = environment_get(env, expr);
	}
	else {
		error(1, 0, "Could not evaluate value 0x%016lX\n: Unknown type.", expr);
	}

	return result;
}


