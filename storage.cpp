#include <iostream>
#include <assert.h>

#include "value.h"

#include "storage.h"

#include "pointer.h"

static const int BLOCK_SIZE = 65520;

double_storage_t* ds_pool = NULL;
single_storage_t* ss_pool = NULL;

uint32_t next_ss = 0;
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

single_storage_t* alloc_single_storage(void) {
	assert(next_ss < BLOCK_SIZE);
	if (!ss_pool) {
		ss_pool = new single_storage_t[BLOCK_SIZE];
		next_ss = 0;
	}
	single_storage_t* result = (single_storage_t*) (ss_pool + next_ss);
	next_ss++;
	return result;
}
