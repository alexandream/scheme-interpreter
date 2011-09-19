#ifndef __VALUE_H__
#define __VALUE_H__

#include <stdint.h>

typedef uint64_t value_t;

extern const
uint8_t MARK_POLICY_NONE,
        MARK_POLICY_FIRST,
        MARK_POLICY_SECOND,
        MARK_POLICY_BOTH,
		GC_ALWAYS_MARKED,
		GC_HAS_MARK,
		GC_IS_IN_USE;


static inline
bool is_immediate(value_t value) {
	return (value & 0x03) != 0;
}

#define GC_DATA_OFFSET 44
#define CLEAR_GC_DATA_MASK ~((uint64_t) 0xFF << GC_DATA_OFFSET)

#include <stdio.h>
// Header:
// abcd: a -> immutable, b c d -> reserved
// T(8): T -> type info
// GC(8): GC -> Garbage Collector Info
//
static inline
uint64_t make_header(bool immutable, uint8_t type, uint8_t mark_policy) {
	uint64_t result = 0;
	if (immutable) {
		result |= 0x8000000000000000;
	}
	result |= ((uint64_t) type) << 52;
	result |= ((uint64_t) (mark_policy|GC_IS_IN_USE)) << GC_DATA_OFFSET;
	return result;
}

static inline
uint8_t header_get_type(uint64_t header) {
	return (uint8_t) (header >> 52);
}

#include "storage.h"
#include "pointer.h"

static inline
uint64_t* get_header(value_t value) {
	return &(((double_storage_t*) unwrap_pointer(value))->header);
}

static inline
uint8_t get_non_immediate_type(value_t value) {
	// TODO: Adapt this for non-builtin types.
	uint64_t* header = get_header(value);
	return header_get_type(*header);
}


static inline 
uint8_t get_gc_data(uint64_t header) {
	return (uint8_t) (header >> GC_DATA_OFFSET);
}

static inline
void set_gc_data(uint64_t* header, uint8_t gc_data) {
	*header &= CLEAR_GC_DATA_MASK;
	*header |= (uint64_t) gc_data << GC_DATA_OFFSET;
}
static inline
void set_gc_mark(uint64_t* header) {
	*header |= (uint64_t) GC_HAS_MARK << GC_DATA_OFFSET;
}

static inline
void clear_gc_mark(uint64_t* header) {
	*header &= ~((uint64_t) GC_HAS_MARK << GC_DATA_OFFSET);
}

static inline
bool has_gc_mark(uint64_t header) {
	return (get_gc_data(header) & (GC_HAS_MARK|GC_ALWAYS_MARKED)) != 0;
}

static inline
bool is_in_use(uint64_t header) {
	return (get_gc_data(header) & GC_IS_IN_USE) != 0;
}


static inline
uint64_t must_mark_first(uint64_t header) {
	return get_gc_data(header) & MARK_POLICY_FIRST;
}

static inline
uint64_t must_mark_second(uint64_t header) {
	return get_gc_data(header) & MARK_POLICY_SECOND;
}


#endif

