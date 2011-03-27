#include <iostream>

#include "cell.h"
#include "int32.h"
#include "boolean.h"
#include "symbol.h"
cell_t value;

int main(int argc, char ** argv) {
	cell_ptr_t cellf = boolean_make_from_string("#F");
	cell_ptr_t cellt = boolean_make_from_string("#T");

	std::cout << boolean_display_format(cellf) << " :: " << boolean_display_format(cellt) << std::endl;
	
	cell_ptr_t cell_pi = int32_make_from_string("11235");
	cell_ptr_t cell_ni = int32_make_from_string("-11235");

	std::cout << int32_display_format(cell_pi) << " :: " << int32_display_format(cell_ni) << std::endl;
	
	cell_ptr_t cell_symbol0 = symbol_make_from_string("hello");
	cell_ptr_t cell_symbol1 = symbol_make_from_string("hello");
	cell_ptr_t cell_symbol2 = symbol_make_from_string("world");

	std::cout << cell_symbol0 << " :: " << cell_symbol1 << " :: " << cell_symbol2 << std::endl;
	std::cout << symbol_display_format(cell_symbol0) << " :: " << 
		         symbol_display_format(cell_symbol2) << std::endl;
	return 0;
}
