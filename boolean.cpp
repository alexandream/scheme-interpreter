#include <sstream>
#include <string>
#include <cassert>

#include "boolean.h"


cell_ptr_t boolean_make_from_string(const std::string& str) {
	// ATTENTION: This assumes that checking whether the string represents a
	// boolean or not was already done by an upper layer, like the lexer.
	
	char second = str[1];
	
	cell_ptr_t result = TRUE;
	if (second == 'f' || second == 'F') {
		result = FALSE;
	}

	return result;
}	


std::string boolean_display_format(cell_ptr_t cell) {
	// ATTENTION: This assumes that checking whether the cell is one of the
	// boolean values is already done by an upper layer, like the dispatcher.
	
	if (cell == TRUE) {
		return "#T";
	}
	else {
		return "#F";
	}
}
