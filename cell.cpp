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


value_t car(value_t cell) {
	// XXX: Assumption #003
	return ((double_storage_t*) unwrap_pointer(cell))->first_slot;
}

value_t cdr(value_t cell) {
	// XXX: Assumption #003
	return ((double_storage_t*) unwrap_pointer(cell))->second_slot;
}
