#include <iostream>
#include <assert.h>

#include "value.h"
#include "special.h"
#include "storage.h"
#include "pointer.h"

static const int BLOCK_SIZE = 2001;

double_storage_t* ds_pool = NULL;

uint64_t free_list;

#include <stdio.h>

double_storage_t* get_double_storage_pool(int* size) {
    *size = BLOCK_SIZE;
    return ds_pool;
}

double_storage_t* make_double_storage_pool(void) {
	
	double_storage_t* result = new double_storage_t[BLOCK_SIZE];
	int i;
	for (i = 0; i < BLOCK_SIZE; i++) {
		result[i].first_slot = (value_t) (result + i + 1);
	}
	result[i].first_slot = 0;
	return result;
}

double_storage_t* pop_free_ds(void) {
	assert(free_list != 0);
	double_storage_t* result = (double_storage_t*) free_list;
	free_list = result->first_slot;
	result->first_slot = result->second_slot = UNDEFINED;
	return result;
}

void init_storage(void) {
    ds_pool = make_double_storage_pool();
	free_list = (value_t) ds_pool;
}

double_storage_t* alloc_double_storage(void) {
	return pop_free_ds();
}

void free_double_storage(double_storage_t* storage) {
    storage->header = 0;
    storage->first_slot = free_list;
    free_list = (uint64_t) storage;
}

