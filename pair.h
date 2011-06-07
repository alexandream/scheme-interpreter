#ifndef __PAIR_H__
#define __PAIR_H__

#include <string>

#include "value.h"
#include "storage.h"
#include "pointer.h"

extern const
uint8_t PAIR_TYPE_MASK;

static inline
bool is_pair(value_t pair) {
	return (!is_immediate(pair) && 
	        get_non_immediate_type(pair) == PAIR_TYPE_MASK);
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

static inline
void pair_set_right(value_t pair, value_t val) {
	((double_storage_t*) unwrap_pointer(pair))->second_slot = val;
}

value_t make_pair(value_t, value_t);
value_t make_list(value_t first, ...);
int32_t pair_linked_length(value_t);

std::string pair_format(value_t pair);
#endif
