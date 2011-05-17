#include <string>
#include <sstream>

#include "primitive_function.h"
#include "pointer.h"

std::string primitive_function_format(value_t value) {
	std::stringstream sstream;

	sstream << "Primitive Function at 0x" << std::hex << unwrap_pointer(value);
	return sstream.str();
}

value_t primitive_function_apply(value_t func, value_t param_list) {
	primitive_function_t f_ptr = (primitive_function_t) unwrap_pointer(func);

	return (*f_ptr)(param_list);
}

