#ifndef __CELL_H__
#define __CELL_H__

#include "value.h"
#include "memory.h"
#include "pointer.h"
static inline
bool is_cons(value_t cell) {
	return ((((uint8_t) cell) & 0x03) == 0x00);
}

static inline
value_t car(value_t cell) {
	// XXX: Assumption #003
	return ((double_storage_t*) unwrap_pointer(cell))->first_slot;
}

static inline
value_t cdr(value_t cell) {
	// XXX: Assumption #003
	return ((double_storage_t*) unwrap_pointer(cell))->second_slot;
}

value_t cons(value_t, value_t);
#endif
