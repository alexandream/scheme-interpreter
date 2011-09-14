#ifndef __VALUE_H__
#define __VALUE_H__

#include <stdint.h>

typedef uint64_t value_t;

extern const
uint8_t MARK_POLICY_NONE,
        MARK_POLICY_FIRST,
        MARK_POLICY_SECOND,
        MARK_POLICY_BOTH;


static inline
bool is_immediate(value_t value) {
	return (value & 0x03) != 0;
}

static inline
uint64_t make_header(bool immutable, uint8_t type, uint8_t mark_policy) {
	uint64_t result = 0;
	if (immutable) {
		result |= 0x8000000000000000;
	}
	result |= ((uint64_t) type) << 52;
	result |= ((uint64_t) mark_policy) << 49;
	return result;
}

static inline
uint8_t header_get_type(uint64_t header) {
	return (uint8_t) (header >> 52);
}

#include "storage.h"
#include "pointer.h"

static inline
uint64_t get_header(value_t value) {
	return ((double_storage_t*) unwrap_pointer(value))->header;
}

static inline
void set_header(value_t value, uint64_t header) {
	((double_storage_t*) unwrap_pointer(value))->header = header;
}
static inline
uint8_t get_non_immediate_type(value_t value) {
	return header_get_type(get_header(value));
}

static inline
void set_gc_mark(value_t value) {
	set_header(value, get_header(value) | 0x0008000000000000);
}


static inline
uint64_t has_gc_mark(value_t value) {
	return get_header(value) & 0x0008000000000000;
}

static inline
uint64_t must_mark_first(value_t value) {
	return get_header(value) & 0x0004000000000000;
}

static inline
uint64_t must_mark_second(value_t value) {
	return get_header(value) & 0x0002000000000000;
}
#endif

