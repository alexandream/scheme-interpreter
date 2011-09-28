#include <string>
#include <iostream>
#include <sstream>
#include <error.h>

#include "value.h"

#include "formatter.h"
#include "special.h"
#include "environment.h"
#include "fixnum.h"
#include "symbol.h"
#include "pair.h"
#include "primitive.h"
#include "function.h"

std::string format(value_t value) {
	std::string result;

	// TODO: This must be a generic function. Not this mess.
	if (is_boolean(value)) {
		result = boolean_format(value);
	}
	else if (is_fixnum(value)) {
		result = fixnum_format(value);
	}
	else if (is_pair(value)) {
		result = pair_format(value);
	}
	else if (is_primitive(value)) {
		result = primitive_format(value);
	}
	else if (is_function(value)) {
		result = function_format(value);
	}
	else if (is_symbol(value)) {
		result = symbol_format(value);
	}
	else if (is_environment(value)) {
		result = environment_format(value);
	}
	else {
		if (value == EMPTY_LIST) 
			result = "()";
		else if (value == UNSPECIFIED)
			result = "#U";
#ifdef DEBUG_COMPILER
		else if (value == OP_HALT)
			result = "@OP_HALT";
		else if (value == OP_CONSTANT)
			result = "@OP_CONSTANT";
		else if (value == OP_LOOKUP)
			result = "@OP_LOOKUP";
		else if (value == OP_CLOSURE)
			result = "@OP_CLOSURE";
		else if (value == OP_TEST)
			result = "@OP_TEST";
		else if (value == OP_ASSIGN)
			result = "@OP_ASSIGN";
		else if (value == OP_SAVE)
			result = "@OP_SAVE";
		else if (value == OP_REIFY)
			result = "@OP_REIFY";
		else if (value == OP_FRAME)
			result = "@OP_FRAME";
		else if (value == OP_APPLY)
			result = "@OP_APPLY";
		else if (value == OP_ARGUMENT)
			result = "@OP_ARGUMENT";
		else if (value == OP_RETURN)
			result = "@OP_RETURN";
		else if (value == OP_BIND)
			result = "@OP_BIND";
		else if (value == OP_BIND_MACRO)
			result = "@OP_BIND_MACRO";
#endif 
		else {
			std::ostringstream sstream;
			sstream << "[Unknown:0x" << std::hex << value;
			if (!is_immediate(value)) {
				sstream << " - " << *get_header(value);
			}
			sstream << "]";
			result = sstream.str();
		}

	}
	return result;
}	


const char* get_type(value_t value) {
	if (is_pair(value)) 
		return "PAIR";
	if (is_primitive(value)) 
		return "PRIMITIVE";
	if (is_function(value)) 
		return "FUNCTION";
	if (is_symbol(value)) 
		return "SYMBOL";
	if (is_environment(value)) 
		return "ENVIRONMENT";
	if (is_immediate(value)) 
		return "IMMEDIATE";
	return "WTF?";
}

