#include <assert.h>
#include <map>
#include <error.h>

#include "environment.h"
#include "symbol.h"
#include "special.h"

typedef std::map<value_t, value_t> binding_map_t;

struct environment_t {
	environment_t *parent;
	binding_map_t map;
};

environment_t *make_environment(environment_t *parent) {
	environment_t *result = new environment_t();
	result->parent = parent;

	return result;
}

void environment_set(environment_t *env, value_t symbol, value_t value) {
	assert(env);
	assert(is_symbol(symbol));
	
	env->map[symbol] = value;
}

value_t environment_get(environment_t *env, value_t symbol) {
	assert(env);
	assert(is_symbol(symbol));

	value_t result = UNDEFINED;
	binding_map_t::iterator i = env->map.find(symbol);
	if (i != env->map.end()) {
		result = i->second;
	}

	if (result == UNDEFINED) {
		if (env->parent) {
		result = environment_get(env->parent, symbol);
		}
		else {
			error(1, 0, "Could not find a binding for symbol '%s' in current environment.",
			      symbol_format(symbol).c_str());
		}
	}
	

	return result;
}
