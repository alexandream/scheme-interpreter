#include <string>
#include <map>

#include "symbol.h"
#include "value.h"

typedef std::map<std::string, value_t> symbol_pool_t;

static
symbol_pool_t symbol_pool;


static inline
value_t wrap_string_pointer(std::string* ptr) {
	// XXX: Assumption #001
	return ((uint64_t) ptr) | 0x01;
}

static inline
void * unwrap_pointer(value_t value) {
	// XXX: Assumption #001
	return (void*) (value & 0xFFFFFFFFFFFFFFF0);
}

value_t symbol_make_from_string(const std::string& str) {
	// XXX: Assumption #002
	symbol_pool_t::iterator iter;
	
	value_t result;
	iter = symbol_pool.find(str);
	if (iter == symbol_pool.end()) {
		std::string *new_str = new std::string(str);
		result = wrap_string_pointer(new_str);
		symbol_pool[str] = result;
	}
	else {
		result = iter->second;
	}
	return result;
}

std::string symbol_format(value_t value) {
	// XXX: Assumption #003
	std::string *ptr = (std::string*) unwrap_pointer(value);
	return *ptr;
}


