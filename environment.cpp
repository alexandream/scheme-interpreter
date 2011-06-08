#include <assert.h>
#include <map>
#include <error.h>

#include "environment.h"
#include "symbol.h"
#include "pair.h"
#include "special.h"

typedef std::map<value_t, value_t> binding_map_t;

struct environment_t {
	environment_t *parent;
	binding_map_t map;
};

environment_t *old_make_environment(environment_t *parent) {
	environment_t *result = new environment_t();
	result->parent = parent;

	return result;
}

inline
void environment_add(environment_t* env, value_t symbol, value_t value) {
	assert(env);
	assert(is_symbol(symbol));

	env->map[symbol] = value;
}

environment_t *make_environment(environment_t* parent,
                                value_t names,
                                value_t values) {
	environment_t *result = new environment_t();
	
	while (is_pair(names) && is_pair(values)) {
		value_t name = pair_left(names);
		value_t value = pair_left(values);
		environment_add(result, name, value);

		names = pair_right(names);
		values = pair_right(values);
	}

	if (is_pair(names) || is_pair(values)) {
		error(1, 0, "Number of variables and values to make new "
				    "environment don't match.");
	}
	return result;
}

static inline
binding_map_t::iterator environment_fetch(environment_t* env, value_t symbol) {
	assert(env);
	assert(is_symbol(symbol));

	value_t result = UNDEFINED;
	binding_map_t::iterator binding = env->map.find(symbol);
	if (binding != env->map.end()) {
		return binding;
	}
	else if (env->parent) {
		return environment_fetch(env->parent, symbol);
	}
	else {
			error(1, 0, "Could not find a binding for symbol '%s' in "
			            "current environment.",
			      symbol_format(symbol).c_str());

			// Never happens. Above function will stop execution. Return below
			// is simply to stop compiler complaints.
			binding_map_t::iterator dummy;
			return dummy;
	}

}

void environment_set(environment_t *env, value_t symbol, value_t value) {
	binding_map_t::iterator i = environment_fetch(env, symbol);

	i->second = value;
}

value_t environment_get(environment_t *env, value_t symbol) {
	binding_map_t::iterator i = environment_fetch(env, symbol);

	return i->second;
}

