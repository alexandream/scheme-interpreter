#include <sstream>
#include <string>

#include "value.h"
#include "special.h"


const
value_t BOOLEAN_TRUE  = 0x0000000000000010,
        BOOLEAN_FALSE = 0x0000000000000031,
        EMPTY_LIST    = 0x0000000000000050,
        UNDEFINED     = 0x0000000000000070,
        UNSPECIFIED   = 0x0000000000000090,
        END_OF_FILE   = 0x00000000000000B0;


value_t boolean_make_from_string(const std::string& str) {
	// ATTENTION: This assumes that checking whether the string
	// represents a boolean or not was already done by an upper
	// layer, like the lexer.
	char second = str[1];

	value_t result = BOOLEAN_FALSE;
	if (second == 't' || second == 'T') {
		result = BOOLEAN_TRUE;
	}

	return result;
}


std::string boolean_format(value_t value) {
	// ATTENTION: This assumes that checking whether the cell is one of the
	// boolean values is already done by an upper layer, like the dispatcher.
	if (value == BOOLEAN_TRUE)
		return "#T";
	else
		return "#F";
}

