#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__
#include <string>

#include "value.h"
extern const
uint8_t PRIMITIVE_TYPE_MASK;

typedef value_t (*primitive_t)(value_t);

struct primitive_descriptor_t {
	std::string name;
	uint8_t arity;
};


static inline
bool is_primitive(value_t value) {
	return (!is_immediate(value) &&
	        get_non_immediate_type(value) == PRIMITIVE_TYPE_MASK);
}


value_t make_primitive(primitive_t func,
                       primitive_descriptor_t* descriptor);

std::string primitive_format(value_t value);

value_t primitive_apply(value_t, value_t);

#endif
