#ifndef __STORAGE_H__
#define __STORAGE_H__

#include "value.h"


struct double_storage_t {
	uint64_t header;
	value_t first_slot;
	value_t second_slot;
};

void free_double_storage(double_storage_t* storage);
double_storage_t* alloc_double_storage(void);
double_storage_t* get_double_storage_pool(int* size);
void init_storage(void);
#endif
