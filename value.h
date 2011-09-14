#ifndef __VALUE_H__
#define __VALUE_H__

#include <stdint.h>

typedef uint64_t value_t;



static inline
bool is_immediate(value_t value) {
	return (value & 0x03) != 0;
}

static inline
uint64_t make_header(bool immutable, uint8_t type) {
	uint64_t result = 0;
	if (immutable) {
		result |= 0x8000000000000000;
	}
	result |= ((uint64_t) type) << 52;
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
uint8_t get_non_immediate_type(value_t value) {
	return header_get_type(get_header(value));
}

#endif

