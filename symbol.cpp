#include <string>
#include <map>

#include "value.h"

#include "symbol.h"
#include "pointer.h"
#include "storage.h"

typedef std::map<std::string, value_t> symbol_pool_t;

const
uint8_t SYMBOL_TYPE_MASK = 0x02;

static
symbol_pool_t symbol_pool;



value_t symbol_make_from_string(const std::string& str) {
	// XXX: Assumption #002
	symbol_pool_t::iterator iter;
	
	value_t result;
	iter = symbol_pool.find(str);
	if (iter == symbol_pool.end()) {
		std::string *new_str = new std::string(str);
		single_storage_t* storage = alloc_single_storage();
		storage->header = make_header(false, SYMBOL_TYPE_MASK);
		storage->first_slot = (uint64_t) new_str;
		result = wrap_pointer(storage);
		symbol_pool[str] = result;
	}
	else {
		result = iter->second;
	}
	return result;
}

std::string symbol_format(value_t value) {
	// XXX: Assumption #003
	single_storage_t* storage = (single_storage_t*) unwrap_pointer(value);
	std::string *str = (std::string*) storage->first_slot;
	return *str;
}


