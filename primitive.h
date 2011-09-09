#ifndef __PRIMITIVE_H__
#define __PRIMITIVE_H__
#include <string>

#include "value.h"
#include "evaluator.h"
extern const
uint8_t PRIMITIVE_TYPE_MASK;

extern const
uint8_t ARITY_ANY;


typedef void (*primitive_t)(context_t*);

struct primitive_descriptor_t {
	std::string name;
	primitive_t handler;
	uint8_t min_arity;
	uint8_t max_arity;
};


static inline
bool is_primitive(value_t value) {
	return (!is_immediate(value) &&
	        get_non_immediate_type(value) == PRIMITIVE_TYPE_MASK);
}


value_t make_primitive(primitive_descriptor_t* descriptor);

std::string primitive_format(value_t value);

void primitive_apply(context_t* context, value_t func);

#endif
