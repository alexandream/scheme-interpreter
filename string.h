#ifndef __STRING_H__
#define __STRING_H__

#include <string>
#include "value.h"

extern const
uint8_t STRING_TYPE_MASK;

value_t string_preprocess(const std::string& str);

std::string string_format(value_t value);

value_t wrap_string(const std::string& str);
std::string unwrap_string(value_t value);
std::string* unwrap_internal_string(value_t value);
void string_finalize(value_t);

static inline
bool is_string(value_t value) {
	return (!is_immediate(value) &&
	        get_non_immediate_type(value) == STRING_TYPE_MASK);
}


#endif
