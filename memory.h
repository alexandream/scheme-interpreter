#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "value.h"

struct double_storage_t {
	uint64_t meta;
	value_t first_slot;
	value_t second_slot;
};

double_storage_t* alloc_double_storage(void);

#endif
