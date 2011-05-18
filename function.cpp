#include <string>
#include <sstream>
#include <error.h>

#include "value.h"
#include "function.h"

#include "pair.h"
#include "special.h"
#include "symbol.h"
#include "storage.h"
#include "pointer.h"

const
uint8_t FUNCTION_TYPE_MASK = 0x04;

static inline
bool is_all_symbols(value_t arg_list);

value_t make_function(value_t arg_list, value_t body) {
	if (!is_all_symbols(arg_list)) {
		error(1, 0, "Argument lists can only be composed of proper lists of symbols.");
	}
	
	double_storage_t* storage = alloc_double_storage();

	storage->header = make_header(true, FUNCTION_TYPE_MASK);
	storage->first_slot = arg_list;
	storage->second_slot = body;

	return wrap_pointer(storage);
}

std::string function_format(value_t value) {
	std::stringstream sstream;
	sstream << "User defined function at 0x" << std::hex
	        << unwrap_pointer(value);

	return sstream.str();
}
	
static inline
bool is_all_symbols(value_t arg_list) {
	value_t iter = arg_list;
	while (is_pair(iter)) {
		if (!is_symbol(pair_left(iter))) {
			return false;
		}
		iter = pair_right(iter);
	}
	return iter == EMPTY_LIST;
}
