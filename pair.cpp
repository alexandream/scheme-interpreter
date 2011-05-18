#include "pair.h"
#include "pointer.h"

#include "memory.h"


value_t make_pair(value_t left, value_t right) {
	double_storage_t* storage = alloc_double_storage();
	
	// TODO: Initialize meta data on storage.
	
	storage->first_slot = left;
	storage->second_slot = right;

	return ((value_t) storage);
}

