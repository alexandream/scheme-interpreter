#include <iostream>

#include <error.h>
#include <assert.h>

#include "evaluator.h"
#include "environment.h"
#include "primitive.h"
#include "function.h"
#include "reader.h"
#include "special.h"
#include "symbol.h"
#include "pair.h"


value_t evaluate_form(value_t expr, environment_t* env);

value_t evaluate(value_t expr, environment_t *env) {
	value_t result = UNSPECIFIED;
	
	if (is_boolean(expr)) {
		result = expr;
	}
	else if (is_symbol(expr)) {
		result = environment_get(env, expr);
	}
	else if (is_pair(expr)) {
		result = evaluate_form(expr, env);
	}
	else {
		error(1, 0, "Could not evaluate value 0x%016lX\n: Unknown type.", expr);
	}

	return result;
}

value_t evaluate_list(value_t expr, environment_t* env) {
	if (expr == EMPTY_LIST) {
		return EMPTY_LIST;
	}
	else {
		return make_pair(evaluate(pair_left(expr), env), evaluate_list(pair_right(expr), env));
	}
}


value_t evaluate_primitive_application(value_t expr, environment_t* env) {
	value_t func = evaluate(pair_left(expr), env);
	
	assert(is_primitive(func));

	value_t param_list = evaluate_list(pair_right(expr), env);
	return primitive_apply(func, param_list);
}


value_t evaluate_quote(value_t expr, environment_t* env) {
	int32_t arguments = pair_linked_length(expr);

	if (arguments != 1) {
		error(1, 0, "Expected 1 argument for 'quote', got %d.", arguments);
	}
	return pair_left(expr);
}


value_t evaluate_if(value_t expr, environment_t* env) {
	int32_t arguments = pair_linked_length(expr);
	if (arguments != 3) {
		error(1, 0, "Expected 3 arguments for 'if', got %d", arguments);
	}
	value_t condition = evaluate(pair_left(expr), env);
	value_t options = pair_right(expr);
	value_t consequence = pair_left(options);
	value_t alternative = pair_left(pair_right(options));
	
	value_t result;
	if (condition == BOOLEAN_TRUE) {
		result = evaluate(consequence, env);
	}
	else {
		result = evaluate(alternative, env);
	}

	return result;
}


value_t evaluate_define(value_t expr, environment_t* env) {
	int32_t arguments = pair_linked_length(expr);
	if (arguments != 2) {
		error(1, 0, "Expected 2 arguments for 'define', got %d", arguments);
	}
	value_t identifier = pair_left(expr);
	if (!is_symbol(identifier)) {
		error(1, 0, "First argument for 'define' must be a symbol");
	}
	value_t value = evaluate(pair_left(pair_right(expr)), env);
	
	environment_t* global_env = GLOBAL_ENVIRONMENT;
	environment_set(global_env, identifier, value);

	return UNSPECIFIED;
}

value_t evaluate_lambda(value_t expr, environment_t* env) {
	int32_t arguments = pair_linked_length(expr);
	if (arguments != 2) {
		error(1, 0, "Expected 2 arguments for 'lambda', got %d", arguments);
	}
	
	value_t arg_list = pair_left(expr);
	value_t body = pair_left(pair_right(expr));

	value_t function = make_function(arg_list, body);

	return function;
}

value_t QUOTE_SYMBOL = UNSPECIFIED;
value_t IF_SYMBOL = UNSPECIFIED;
value_t DEFINE_SYMBOL = UNSPECIFIED;
value_t LAMBDA_SYMBOL = UNSPECIFIED;

value_t evaluate_form(value_t expr, environment_t*env) {
	// TODO: Take this initialization from here and put it in a place that
	// is run only once.
	if (QUOTE_SYMBOL == UNSPECIFIED) {
		QUOTE_SYMBOL = make_symbol("quote");
	}
	if (IF_SYMBOL == UNSPECIFIED) {
		IF_SYMBOL = make_symbol("if");
	}
	if (DEFINE_SYMBOL == UNSPECIFIED) {
		DEFINE_SYMBOL = make_symbol("define");
	}
	if (LAMBDA_SYMBOL == UNSPECIFIED) {
		LAMBDA_SYMBOL = make_symbol("lambda");
	}

	value_t result = UNSPECIFIED;
	value_t head = pair_left(expr);
	if (head == IF_SYMBOL) {
		result = evaluate_if(pair_right(expr), env);
	}
	else if (head == LAMBDA_SYMBOL) {
		result = evaluate_lambda(pair_right(expr), env);
	}
	else if (head == DEFINE_SYMBOL) {
		result = evaluate_define(pair_right(expr), env);
	}
	else if (head == QUOTE_SYMBOL) {
		result = evaluate_quote(pair_right(expr), env);
	}
	else {
		result = evaluate_primitive_application(expr, env);
	}
	return result;
}

