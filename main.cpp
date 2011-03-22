#include <iostream>

#include "cell.h"
#include "int32.h"

cell_t value;

int main(int argc, char ** argv) {

	int32_init_cell_from_string(value, "-11235");

	std::cout << int32_display_format(value) << std::endl;

	return CELL_CONTENT_MASK;
}
