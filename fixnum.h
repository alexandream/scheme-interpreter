#ifndef __FIXNUM_H__
#define __FIXNUM_H__

#include <stdint.h>

#include "value.h"

/*********************************************************************
 * FIXNUM Representation.                                             *
 *********************************************************************
 * An fixnum is represented as a immediate value in the value_t bits
 * themselves. As such it has a bottom-most tag of IMMED_VALUE_TAG.
 * It's type tag is extended by a 0 in the fifth bit, being the only
 * immediate value with the tag "01111"
 */

static inline 
bool is_fixnum(value_t value) {
	return (value & 0x000000000000001F) == 0x0F;
}

static inline
int64_t unwrap_fixnum(value_t value) {
	return ( (int64_t) value >> 5 );
}

static inline
value_t wrap_fixnum(int64_t number) {
	return  ((value_t) number << 5) | 0x0F;
}
#endif

