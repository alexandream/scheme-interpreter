#include "cell.h"
#include "pointer.h"

#include "memory.h"


value_t cons(value_t car, value_t cdr) {
	double_storage_t* storage = alloc_double_storage();
	
	// TODO: Initialize meta data on storage.
	
	storage->first_slot = car;
	storage->second_slot = cdr;

	return ((value_t) storage);
}


