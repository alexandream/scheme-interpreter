#include <string>
#include <iostream>

#include "printer.h"
#include "formatter.h"

static
void print_pair(value_t pair);
void print(value_t value) {
	std::cout << format(value);
}

void println(value_t value) {
	print(value);
	std::cout << std::endl;
}
