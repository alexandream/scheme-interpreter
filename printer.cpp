#include <string>
#include <iostream>
#include <error.h>

#include "printer.h"
#include "special.h"
#include "symbol.h"
#include "cell.h"
#include "primitive_function.h"

static
void print_cell(value_t cell);
void print(value_t value) {
	if (is_cons(value)) {
		print_cell(value);
	}
	else {
		std::string result;

		// TODO: This must be a generic function. Not this mess.
		if (is_boolean(value)) {
			result = boolean_format(value);
		}
		else if (is_primitive_function(value)) {
			result = primitive_function_format(value);
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
void print_cell(value_t cell) {
	// XXX: Assertion #003
	std::cout << "(";
	do {
		print(car(cell));
		std::cout << " ";
		cell = cdr(cell);
	} while (is_cons(cell));
	std::cout << ")";
}
