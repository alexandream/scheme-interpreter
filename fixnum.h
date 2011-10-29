#ifndef __FIXNUM_H__
#define __FIXNUM_H__

#include <string>
#include "value.h"

extern const
value_t FIXNUM_MAX,
        FIXNUM_MIN;

value_t fixnum_make_from_string(const std::string& str);
value_t fixnum_preprocess(const std::string& original_str);
std::string fixnum_format(value_t value, char radix='d');

static inline
bool is_fixnum(value_t value) {
	return (value & 0x01) != 0;
}

static inline
value_t wrap_fixnum(int64_t fixnum) {
	return ((value_t) fixnum << 1) | 0x01;
}

static inline
int64_t unwrap_fixnum(value_t value) {
	return ((int64_t) value >> 1);
}

#endif
