#include <string>
#include <iostream>
#include <error.h>

#include "value.h"

#include "formatter.h"
#include "special.h"
#include "symbol.h"
#include "pair.h"
#include "primitive.h"

std::string format(value_t value) {
	std::string result;

	// TODO: This must be a generic function. Not this mess.
	if (value == EMPTY_LIST) {
		result = "()";
	}
	else if (value == UNSPECIFIED) {
		result = "#U";
	}
	else if (is_boolean(value)) {
		result = boolean_format(value);
	}
	else if (is_pair(value)) {
		result = pair_format(value);
	}
	else if (is_primitive(value)) {
		result = primitive_format(value);
	}
	else if (is_symbol(value)) {
		result = symbol_format(value);
	}
	else {
		error(1, 0, "Unable to format! Unknown value: 0x%016lX\n", value);
	}
	return result;
}	

