#ifndef __PRIMITIVE_FUNCTION_H__
#define __PRIMITIVE_FUNCTION_H__
#include <string>

#include "value.h"
typedef value_t (*primitive_function_t)(value_t);

static inline
bool is_primitive_function(value_t value) {
	// FIXME: This assumes primitive functions have a tagged
	// space of their own. This is not true, as they'll be
	// atoms like any others later.
	return (((uint8_t) value) & 0x0F) == 0x03;
}

static inline
value_t wrap_primitive_function(primitive_function_t func) {
	return ((uint64_t) func) | 0x03;
}

std::string primitive_function_format(value_t value);

value_t primitive_function_apply(value_t, value_t);

#endif
