#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include <string>
#include "value.h"

extern const
value_t CHARACTER_MAX,
        CHARACTER_MIN;

value_t character_make_from_string(const std::string& str);
value_t character_preprocess(const std::string& original_str);
std::string character_format(value_t value);

static inline
bool is_character(value_t value) {
	return (value & 0x0FF0F) == 0x00002;
}

static inline
value_t wrap_character(char character) {
	return ((value_t) character << 32) | 0x02;
}

static inline
int64_t unwrap_character(value_t value) {
	return ((int64_t) value >> 32);
}

#endif
