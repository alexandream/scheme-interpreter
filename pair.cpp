#include <string>
#include <sstream>

#include "value.h"

#include "pair.h"
#include "pointer.h"
#include "special.h"
#include "storage.h"

#include "formatter.h"

const
uint8_t PAIR_TYPE_MASK = 0x01;

value_t make_pair(value_t left, value_t right) {
	double_storage_t* storage = alloc_double_storage();
	
	storage->header = make_header(false, PAIR_TYPE_MASK);	
	storage->first_slot = left;
	storage->second_slot = right;

	return wrap_pointer(storage);
}


std::string pair_format(value_t pair) {
	// XXX: Assertion #003
	
	std::stringstream sstream;
	sstream << "(";
	do {
		sstream << format(pair_left(pair));
		pair = pair_right(pair);
		if (pair != EMPTY_LIST) {
			sstream << " ";
		}
	} while (is_pair(pair));
	sstream << ")";

	return sstream.str();
}
