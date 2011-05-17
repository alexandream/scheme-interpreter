#include <iostream>
#include <assert.h>

#include "memory.h"
#include "pointer.h"

static const int BLOCK_SIZE = 65520;

double_storage_t* ds_pool = NULL;
uint32_t next_ds = 0;

double_storage_t* alloc_double_storage(void) {
	assert(next_ds < BLOCK_SIZE);
	if (!ds_pool) {
		ds_pool = new double_storage_t[BLOCK_SIZE];
		next_ds = 0;
	}
	double_storage_t* result = (double_storage_t*) (ds_pool + next_ds);
	next_ds++;
	return result;
}

