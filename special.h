#ifndef __SPECIAL_H__
#define __SPECIAL_H__

#include "value.h"

/*********************************************************************
 * SPECIAL Representation.                                           *
 *********************************************************************
 * Every SPECIAL value is represented as an immediate value, hence being
 * stored on the value_t's own bits.
 *
 * As of now, we have 5 distinct special values:
 * - Boolean True: #T
 * - Boolean False: #F
 * - Empty List or NIL: '()
 * - Undefined: No representation. Reserved for future use.
 * - Unspecified: #U
 *
 * The two values for booleans are:
 * #T : 00000000 0x00 0x00 0x00 0x00 0x00 0x00 00010000
 * #F : 00000000 0x00 0x00 0x00 0x00 0x00 0x00 00110000
 *
 * The value for the Empty List:
 *      00000000 0x00 0x00 0x00 0x00 0x00 0x00 01010000
 *
 * The value for Unspecified:
 * #U : 00000000 0x00 0x00 0x00 0x00 0x00 0x00 01110000 
 *
 * The value for Undefined:
 *      00000000 0x00 0x00 0x00 0x00 0x00 0x00 10010000
*/
extern const
value_t BOOLEAN_TRUE,
        BOOLEAN_FALSE,
        EMPTY_LIST,
        UNDEFINED,
        UNSPECIFIED;

static inline 
bool is_boolean(value_t value) {
	return (value == BOOLEAN_TRUE || value == BOOLEAN_FALSE);
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
