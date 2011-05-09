#include <string>
#include <iostream>
#include <error.h>

#include "printer.h"
#include "special.h"

void print(value_t value) {
	std::string result;

	// TODO: This must be a generic function. Not this mess.
	if (is_boolean(value)) {
		result = boolean_format(value);
	}
	else {
		error(1, 0, "Unable to print! Unknown value: 0x%016lX\n", value);
	}
	std::cout << result << std::endl;
}


