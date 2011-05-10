#include <error.h>

#include "evaluator.h"
#include "reader.h"
#include "special.h"
#include "symbol.h"


value_t evaluate(value_t expr) {
	value_t result = UNSPECIFIED;
	
	if (is_boolean(expr)) {
		result = expr;
	}
	else if (is_symbol(expr)) {
		result = expr;
	}
	else {
		error(1, 0, "Could not evaluate value 0x%016lX\n: Unknown type.", expr);
	}

	return result;
}


