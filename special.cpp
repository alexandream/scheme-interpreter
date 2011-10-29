#include <sstream>
#include <string>

#include "value.h"
#include "special.h"

const
value_t UNDEFINED         = 0x0000000000000006,
        UNSPECIFIED       = 0x000000000000000E,
        BOOLEAN_FALSE     = 0x0000000000000016,
        BOOLEAN_TRUE      = 0x000000000000001E,
        END_OF_FILE       = 0x0000000000000026,
        EMPTY_LIST        = 0x000000000000002E,

        OP_HALT           = 0x0000000000000036,
        OP_CONSTANT       = 0x000000000000003E,
        OP_LOOKUP         = 0x0000000000000046,
        OP_CLOSURE        = 0x000000000000004E,
        OP_TEST           = 0x0000000000000056,
        OP_ASSIGN         = 0x000000000000005E,
        OP_SAVE           = 0x0000000000000066,
        OP_REIFY          = 0x000000000000006E,
        OP_FRAME          = 0x0000000000000076,
        OP_APPLY          = 0x000000000000007E,
        OP_ARGUMENT       = 0x0000000000000086,
        OP_RETURN         = 0x000000000000008E,
		OP_BIND           = 0x0000000000000096,
        OP_BIND_MACRO     = 0x000000000000009E;

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

