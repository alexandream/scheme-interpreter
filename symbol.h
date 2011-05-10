#ifndef __SYMBOL_H__
#define __SYMBOL_H__
#include <string>

#include "value.h"

static inline
bool is_symbol(value_t value) {
	// FIXME: This assumes the only value with tag 0x01 is a symbol. Not true.
	return (((uint8_t) value) & 0x0F) == 0x01;
}

value_t symbol_make_from_string(const std::string& str);

std::string symbol_format(value_t value);

#endif
