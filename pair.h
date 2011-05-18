#ifndef __PAIR_H__
#define __PAIR_H__

#include "value.h"
#include "storage.h"
#include "pointer.h"
static inline
bool is_pair(value_t pair) {
	return ((((uint8_t) pair) & 0x03) == 0x00);
}

static inline
bool is_atom(value_t pair) {
	return (! is_pair(pair));
}

static inline
value_t pair_left(value_t pair) {
	// XXX: Assumption #003
	return ((double_storage_t*) unwrap_pointer(pair))->first_slot;
}

static inline
value_t pair_right(value_t pair) {
	// XXX: Assumption #003
	return ((double_storage_t*) unwrap_pointer(pair))->second_slot;
}

value_t make_pair(value_t, value_t);
#endif
