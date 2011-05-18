#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "value.h"

struct single_storage_t {
	uint64_t header;
	value_t slot;
};

struct double_storage_t {
	uint64_t header;
	value_t first_slot;
	value_t second_slot;
};

double_storage_t* alloc_double_storage(void);

#endif
