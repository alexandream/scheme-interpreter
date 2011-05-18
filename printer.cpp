#include <string>
#include <iostream>
#include <error.h>

#include "printer.h"
#include "special.h"
#include "symbol.h"
#include "pair.h"
#include "primitive.h"

static
void print_pair(value_t pair);
void print(value_t value) {
	if (is_pair(value)) {
		print_pair(value);
	}
	else {
		std::string result;

		// TODO: This must be a generic function. Not this mess.
		if (is_boolean(value)) {
			result = boolean_format(value);
		}
		else if (is_primitive(value)) {
			result = primitive_format(value);
		}
		else if (is_symbol(value)) {
			result = symbol_format(value);
		}
		else {
			error(1, 0, "Unable to print! Unknown value: 0x%016lX\n", value);
		}
		std::cout << result;
	}
}

void println(value_t value) {
	print(value);
	std::cout << std::endl;
}

static
void print_pair(value_t pair) {
	// XXX: Assertion #003
	std::cout << "(";
	do {
		print(pair_left(pair));
		std::cout << " ";
		pair = pair_right(pair);
	} while (is_pair(pair));
	std::cout << ")";
}
