#include <assert.h>
#include <error.h>

#include "environment.h"
#include "symbol.h"
#include "pair.h"
#include "special.h"

const
uint8_t ENVIRONMENT_TYPE_MASK = 0x05;


static inline
binding_map_t* get_bindings(value_t env) {
	double_storage_t* storage = (double_storage_t*) unwrap_pointer(env);
	return (binding_map_t*) unwrap_pointer(storage->second_slot);
}

static inline
value_t get_parent(value_t env) {
	double_storage_t* storage = (double_storage_t*) unwrap_pointer(env);
	return storage->first_slot;
}

void environment_add(value_t env, value_t symbol, value_t value) {
	assert(is_symbol(symbol));
	
	binding_map_t* bindings = get_bindings(env);

	(*bindings)[symbol] = value;
}

value_t make_environment(value_t parent, value_t names, value_t values) {
	binding_map_t *bindings = new binding_map_t();
	
	double_storage_t* storage = alloc_double_storage();
	// FIXME: When we add support to garbage collection, we must inform the GC
	// that this object is such that it's second slot must not be "marked", and
	// that it must be deleted when this storage is reclaimed.
	storage->header = make_header(false, ENVIRONMENT_TYPE_MASK);
	
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

	if (is_pair(names) || is_pair(values)) {
		error(1, 0, "Number of variables and values to make new "
				    "environment don't match.");
	}

	return wrap_pointer(storage);
}

static inline
binding_map_t::iterator environment_fetch(value_t env, value_t symbol) {
	assert(env);
	assert(is_symbol(symbol));

	binding_map_t* bindings = get_bindings(env);
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

void environment_set(value_t env, value_t symbol, value_t value) {
	binding_map_t::iterator i = environment_fetch(env, symbol);

	i->second = value;
}

value_t environment_get(value_t env, value_t symbol) {
	binding_map_t::iterator i = environment_fetch(env, symbol);

	return i->second;
}

