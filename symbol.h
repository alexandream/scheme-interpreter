#ifndef __SYMBOL_H__
#define __SYMBOL_H__
#include <string>

#include "value.h"

extern const
uint8_t SYMBOL_TYPE_MASK;

static inline
bool is_symbol(value_t value) {
	return (!is_immediate(value) &&
			get_non_immediate_type(value) == SYMBOL_TYPE_MASK);
}

value_t symbol_make_from_string(const std::string& str);

std::string symbol_format(value_t value);

#endif
