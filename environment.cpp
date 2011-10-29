#include <assert.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <error.h>

#include "environment.h"
#include "symbol.h"
#include "pair.h"
#include "printer.h"
#include "special.h"

const
uint8_t ENVIRONMENT_TYPE_MASK = 0x05;



static inline
value_t get_parent(value_t env) {
	double_storage_t* storage = (double_storage_t*) unwrap_pointer(env);
	return storage->first_slot;
}

void environment_add(value_t env, value_t symbol, value_t value) {
	assert(is_symbol(symbol));
	
	binding_map_t* bindings = environment_get_all_bindings(env);

	(*bindings)[symbol] = value;
}

value_t make_environment(value_t parent, value_t names, value_t values) {
	binding_map_t *bindings = new binding_map_t();
	
	double_storage_t* storage = alloc_double_storage();
	// XXX: mark policy here is useless. Marking of environments is handled
	// in a special way.
	storage->header = make_header(false, ENVIRONMENT_TYPE_MASK, MARK_POLICY_NONE);
	
	storage->first_slot = parent;
	storage->second_slot = wrap_pointer((void*) bindings);
	while (is_pair(names) && is_pair(values)) {
		value_t name = pair_left(names);
		value_t value = pair_left(values);
		
		assert(is_symbol(name));
		(*bindings)[name] = value;

		names = pair_right(names);
		values = pair_right(values);
	}
	if (is_symbol(names)) {
		(*bindings)[names] = values;
	}
	else if (is_pair(values)) {
		error(1, 0, "Received more arguments than parameters to make"
				"new environment");
	}
	else if (is_pair(names)) {
		error(1, 0, "Received more parameters than arguments to make new"
		            "environment");
	}
	value_t result = wrap_pointer(storage);
	//printf("Making environment at 0x%Lx\n", result);
	return result;
}
void environment_finalize(value_t environment) {
	double_storage_t* storage = (double_storage_t*) 
	                              unwrap_pointer(environment);
	binding_map_t *bindings = (binding_map_t*) storage->second_slot;
	delete bindings;
}

static inline
binding_map_t::iterator environment_fetch(value_t env, value_t symbol) {
	assert(env);
	assert(is_symbol(symbol));

	binding_map_t* bindings = environment_get_all_bindings(env);
	value_t parent = get_parent(env);

	binding_map_t::iterator binding = bindings->find(symbol);
	if (binding != bindings->end()) {
		return binding;
	}
	else if (parent != UNSPECIFIED) {
		return environment_fetch(parent, symbol);
	}
	else {
			error(1, 0, "Could not find a binding for symbol '%s' in "
			            "current environment.",
			      symbol_format(symbol).c_str());

			// Never happens. The function above won't return. Return below
			// is simply to stop compiler complaints.
			binding_map_t::iterator dummy;
			return dummy;
	}

}

std::string environment_format(value_t environment) {
	std::ostringstream sstream;
	sstream << "[ Environment (0x" << std::hex << environment << ") with bindings: ";
	binding_map_t* bindings = environment_get_all_bindings(environment);
	binding_map_t::iterator iter = bindings->begin();
	while(iter != bindings->end()) {
		sstream << symbol_format(iter->first) << " ";
		iter++;
	}
	sstream << " ]";
	return sstream.str();
}

void environment_set(value_t env, value_t symbol, value_t value) {
	binding_map_t::iterator i = environment_fetch(env, symbol);

	i->second = value;
}

value_t environment_get(value_t env, value_t symbol) {
	binding_map_t::iterator i = environment_fetch(env, symbol);

	return i->second;
}

