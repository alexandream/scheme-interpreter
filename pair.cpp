
#include "value.h"

#include "pair.h"
#include "pointer.h"
#include "special.h"
#include "storage.h"

const
uint8_t PAIR_TYPE_MASK = 0x01;

value_t make_pair(value_t left, value_t right) {
	double_storage_t* storage = alloc_double_storage();
	
	storage->header = make_header(false, PAIR_TYPE_MASK);	
	storage->first_slot = left;
	storage->second_slot = right;

	return wrap_pointer(storage);
}

