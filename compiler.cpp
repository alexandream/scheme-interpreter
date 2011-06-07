#include <error.h>

#include "value.h"

#include "pair.h"
#include "special.h"
#include "symbol.h"

static
value_t SYM_QUOTE = UNSPECIFIED,
        SYM_IF = UNSPECIFIED,
        SYM_DEFINE = UNSPECIFIED,
        SYM_LAMBDA = UNSPECIFIED;


value_t compile_if(value_t expr, value_t next);
value_t compile_lambda(value_t expr, value_t next);
value_t compile_quote(value_t expr, value_t next);
value_t compile_application(value_t expr, value_t next);
value_t compile_form(value_t expr, value_t next);

value_t compile(value_t expr, value_t next) {
	value_t result;

	if (is_symbol(expr)) {
		result = make_list(OP_LOOKUP, expr, next, 0);
	}
	else if (is_pair(expr)) {
		result = compile_form(expr, next);	
	}
	else {
		result = make_list(OP_CONSTANT, expr, next, 0);
	}
	return result;
}


value_t compile_form(value_t expr, value_t next) {
	if (SYM_QUOTE == UNSPECIFIED) {
		SYM_QUOTE = make_symbol("quote");
		SYM_IF = make_symbol("if");
		SYM_DEFINE = make_symbol("define");
		SYM_LAMBDA = make_symbol("lambda");
	}
	
	value_t head = pair_left(expr);
	value_t result;
	if (head == SYM_IF) {
		result = compile_if(pair_right(expr), next);
	}
	else if (head == SYM_LAMBDA) {
		result = compile_lambda(pair_right(expr), next);
	}
	else if (head == SYM_QUOTE) {
		result = compile_quote(pair_right(expr), next);
	}
	else {
		result = compile_application(expr, next);
	}
	return result;

}


value_t compile_lambda(value_t expr, value_t next) {
	// TODO: Support implicit begin block inside lambda.
	int32_t arguments = pair_linked_length(expr);
	if (arguments != 2) {
		error(1, 0, "Expected 2 arguments for 'lambda', got %d", arguments);
	}
	
	value_t arg_list = pair_left(expr);
	value_t body = pair_left(pair_right(expr));

	value_t compiled_body = compile(body, make_list(OP_RETURN, 0));

	return make_list(OP_CLOSURE, arg_list, compiled_body, 0);
}


value_t compile_if(value_t expr, value_t next) {
	int32_t arguments = pair_linked_length(expr);
	if (arguments < 2 || arguments > 3) {
		error(1, 0, "Expected 2 or 3 arguments for 'if', got %d", arguments);
	}
	value_t condition = pair_left(expr);
	value_t options = pair_right(expr);
	value_t consequence = pair_left(options);
	value_t alternative = UNSPECIFIED;
	if (arguments == 3) {
		alternative = pair_left(pair_right(options));
	}
	value_t compiled_consequence = compile(consequence, next);
	value_t compiled_alternative = compile(alternative, next);

	value_t if_continuation = make_list(OP_TEST,
	                                    compiled_consequence,
	                                    compiled_alternative, 0);

	return compile(condition, if_continuation);
}

value_t compile_quote(value_t expr, value_t next) {
	int32_t arguments = pair_linked_length(expr);

	if (arguments != 1) {
		error(1, 0, "Expected 1 argument for 'quote', got %d.", arguments);
	}
	value_t quoted_obj = pair_right(expr);
	return make_list(OP_CONSTANT, quoted_obj, next, 0);
}

value_t compile_application(value_t expr, value_t next) {
	int32_t arguments = pair_linked_length(expr);

	if (arguments < 1) {
		error(1, 0, "Expected at least 1 argument for function application, "
				    "got none.");
	}
	value_t oper = pair_left(expr);
	value_t apply = make_list(OP_APPLY, 0);
	
	value_t result = compile(oper, apply);
	
	value_t args = pair_right(expr);

	while (args != EMPTY_LIST) {
		result = compile(pair_left(args), make_list(OP_ARGUMENT, result, 0));
		args = pair_right(args);
	}

	// Detecting tail calls.
	value_t next_op = pair_left(next);
	if (next_op != OP_RETURN && next_op != OP_HALT) {
		// If we're not making a tail call, we should save the current frame
		// before doing all the function-application code generated above.
		result = make_list(OP_FRAME, next, result, 0);
	}

	return result;
}

