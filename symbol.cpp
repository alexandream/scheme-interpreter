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



value_t make_symbol(const std::string& str) {
	// XXX: Assumption #002
	symbol_pool_t::iterator iter;
	
	value_t result;
	iter = symbol_pool.find(str);
	if (iter == symbol_pool.end()) {
		std::string *new_str = new std::string(str);
		double_storage_t* storage = alloc_double_storage();
		storage->header = make_header(false, SYMBOL_TYPE_MASK,
		                              MARK_POLICY_NONE|GC_ALWAYS_MARKED);
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
	double_storage_t* storage = (double_storage_t*) unwrap_pointer(value);
	std::string *str = (std::string*) storage->first_slot;
	return *str;
}

size_t get_symbol_pool_size(void) {
	return symbol_pool.size();
}

