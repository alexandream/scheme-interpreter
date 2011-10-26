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


value_t make_function(value_t env, value_t params, value_t body) {
	protect_value(params);
	protect_value(body);
	protect_value(env);
	double_storage_t* storage = alloc_double_storage();

	storage->header = make_header(true, FUNCTION_TYPE_MASK, MARK_POLICY_BOTH);
	storage->first_slot = env;

	protect_storage(storage);
	storage->second_slot = make_pair(params, body);
	unprotect_storage(4);
	
	return wrap_pointer(storage);
}

std::string function_format(value_t value) {
	std::stringstream sstream;
	sstream << "User defined function at 0x" << std::hex
	        << unwrap_pointer(value);
	
	return sstream.str();
}
/*
value_t function_apply(value_t func, value_t param_list, value_t env) {
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
		env = make_environment(env, arg_list, param_list);
	}
	return UNSPECIFIED;//evaluate(body, env);
}
*/

