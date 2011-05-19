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
#include "evaluator.h"
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
	
value_t function_apply(value_t func, value_t param_list, environment_t* env) {
	double_storage_t* storage = (double_storage_t*) unwrap_pointer(func);

	value_t arg_list = storage->first_slot;
	value_t body = storage->second_slot;
	
	int32_t n_args = pair_linked_length(arg_list);
	int32_t n_params = pair_linked_length(param_list);
	if (n_args != n_params) {
		error(1, 0, "User defined function expected %d arguments, got %d",
		            n_args,
		            n_params);
	}
	if (n_args > 0) {
		env = make_environment(env);
		value_t i_arg = arg_list;
		value_t i_param = param_list;
		while(is_pair(i_arg)) {
			// TODO: Swap names on 'arg' and 'param'. They're inverted.
			value_t arg = pair_left(i_arg);
			value_t param = pair_left(i_param);
			environment_set(env, arg, param);
			i_arg = pair_right(i_arg);
			i_param = pair_right(i_param);
		}
	}
	return evaluate(body, env);
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
