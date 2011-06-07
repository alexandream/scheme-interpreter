#ifndef __SPECIAL_H__
#define __SPECIAL_H__
#include <string>

#include "value.h"

extern const
value_t BOOLEAN_TRUE,
        BOOLEAN_FALSE,
        EMPTY_LIST,
        UNDEFINED,
        UNSPECIFIED,
		END_OF_FILE,
        OP_HALT,
        OP_CONSTANT,
        OP_LOOKUP,
        OP_CLOSURE,
        OP_TEST,
        OP_ASSIGN,
        OP_SAVE,
        OP_REIFY,
        OP_FRAME,
        OP_APPLY,
        OP_ARGUMENT,
        OP_RETURN;
static inline
bool is_special(value_t value) {
	return ((value & 0x0000000000000006) == 0x06);
}

static inline 
bool is_boolean(value_t value) {
	return ((value & 0xFFFFFFFFFFFFFFF6) == 0x16);
}

static inline
bool unwrap_boolean(value_t value) {
	return value == BOOLEAN_TRUE;
}

static inline
value_t wrap_boolean(bool boolean) {
	return (boolean ? BOOLEAN_TRUE : BOOLEAN_FALSE);
}

value_t boolean_make_from_string(const std::string& str);
std::string boolean_format(value_t value);


#endif
