#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__
#include <string>

#include "value.h"
typedef value_t (*primitive_t)(value_t);

static inline
bool is_primitive(value_t value) {
	// FIXME: This assumes primitive functions have a tagged
	// space of their own. This is not true, as they'll be
	// atoms like any others later.
	return (((uint8_t) value) & 0x0F) == 0x03;
}

static inline
value_t wrap_primitive(primitive_t func) {
	return ((uint64_t) func) | 0x03;
}

std::string primitive_format(value_t value);

value_t primitive_apply(value_t, value_t);

#endif
