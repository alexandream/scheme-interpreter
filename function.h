#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include <string>

#include "value.h"
#include "environment.h"

extern const
uint8_t FUNCTION_TYPE_MASK;


static inline
bool is_function(value_t value) {
	return (!is_immediate(value) && 
	        get_non_immediate_type(value) == FUNCTION_TYPE_MASK);
}


value_t make_function(value_t, value_t);

std::string function_format(value_t value);

value_t function_apply(value_t, value_t, value_t);

#endif
