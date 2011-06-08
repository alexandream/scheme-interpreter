#include <string>
#include <iostream>

#include "printer.h"
#include "formatter.h"

void print(value_t value, const char* prefix) {
	std::cout << prefix << format(value);
}

void println(value_t value, const char* prefix) {
	print(value, prefix);
	std::cout << std::endl;
}
