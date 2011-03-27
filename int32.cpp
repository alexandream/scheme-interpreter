#include <sstream>
#include <string>
#include <cassert>

#include "int32.h"


cell_ptr_t make_int32_cell(int32_t value) {
	cell_ptr_t result;
	cell_t& cell = cell_alloc(result);

	META_TYPE(cell, CELL_META_TYPE_SIMPLE_ATOM);
	cell.cdr = (uint32_t) value;

	return result;
}

cell_ptr_t int32_make_from_string(const std::string& str) {
	// XXX: This assumes that checking whether the string represents a boolean
	// or not was already done by an upper layer, like the lexer.

	int32_t num;
	std::stringstream ss(str);
	
	ss >> num;
	cell_ptr_t result = make_int32_cell(num);
	return result;
}	

std::string int32_display_format(cell_ptr_t cell) {
	// XXX: This assumes that checking whether the cell is one of the boolean
	// values is already done by an upper layer, like the dispatcher.
	
	std::ostringstream oss;
	oss << (int32_t) CDR(cell);
	return oss.str();
}
