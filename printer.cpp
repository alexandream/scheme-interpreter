#include <string>
#include <iostream>
#include <error.h>

#include "printer.h"

void print(value_t value) {
	error(1, 0, "Unable to print! Unknown value: 0x%016lX\n", value);
}


