#include <sstream>
#include <string>
#include <cassert>

#include "int32.h"


void int32_init_cell_from_string(cell_t& result, const std::string& str) {
	int32_t num;
	std::stringstream ss(str);
	
	assert(!(ss >> num).fail());

	CELL_SET_META_TYPE(result, CELL_META_TYPE_SIMPLE_ATOM);
	result.cdr = (uint32_t) num;
}	



std::string int32_display_format(const cell_t& cell) {
	assert(CELL_META_TYPE(cell) == CELL_META_TYPE_SIMPLE_ATOM);
	/* TODO: Check int32 type here, as well as being a SIMPLE_ATOM.
	assert(CELL_CAR(cell) == INT32_TYPE);
	*/
	
	std::ostringstream oss;
	oss << (int32_t) cell.cdr;
	return oss.str();
}
