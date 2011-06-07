#include <string>
#include <sstream>
#include <stdarg.h>

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


int32_t pair_linked_length(value_t value) {
	int32_t result = 0;
	while (is_pair(value)) {
		result ++;
		value = pair_right(value);
	}
	return result;
}

value_t make_list(value_t first, ...) {
	va_list args;
	value_t list = make_pair(first, EMPTY_LIST);
	value_t last = list;
	va_start(args, first);
	value_t next = va_arg(args, value_t);
	value_t link;
	while (next != 0) {
		link = make_pair(next, EMPTY_LIST);
		pair_set_right(last, link);
		last = link;
		next = va_arg(args, value_t);
	}
	return list;
}
