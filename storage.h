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

void protect_value(value_t value);
void protect_storage(double_storage_t* storage);
void unprotect_storage(int count);
value_list_t* list_protected_values(void);
#endif
