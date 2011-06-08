#ifndef __FUNCTION_H__
#define __FUNCTION_H__
#include <string>

#include "value.h"
#include "pair.h"

extern const
uint8_t FUNCTION_TYPE_MASK;


static inline
bool is_function(value_t value) {
	return (!is_immediate(value) && 
			get_non_immediate_type(value) == FUNCTION_TYPE_MASK);
}

static inline
value_t function_get_environment(value_t function) {
	return ((double_storage_t*) unwrap_pointer(function))->first_slot;
}

static inline
value_t function_get_data(value_t function) {
	return ((double_storage_t*) unwrap_pointer(function))->second_slot;
}

static inline
value_t function_get_params(value_t function) {
	return pair_left(function_get_data(function));
}

static inline
value_t function_get_body(value_t function) {
	return pair_right(function_get_data(function));
}

value_t make_function(value_t, value_t, value_t);

std::string function_format(value_t value);

value_t function_apply(value_t, value_t, value_t);

#endif
