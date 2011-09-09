#include <error.h>

#include "value.h"

#include "pair.h"
#include "special.h"
#include "symbol.h"

static
value_t SYM_QUOTE = UNSPECIFIED,
        SYM_IF = UNSPECIFIED,
        SYM_DEFINE = UNSPECIFIED,
        SYM_LAMBDA = UNSPECIFIED,
		SYM_BEGIN = UNSPECIFIED;


value_t compile_if(value_t expr, value_t next);
value_t compile_begin(value_t expr, value_t next);
value_t compile_sequence(value_t expr_list, value_t next);
value_t compile_lambda(value_t expr, value_t next);
value_t compile_quote(value_t expr, value_t next);
value_t compile_application(value_t expr, value_t next);
value_t compile_define(value_t args, value_t next);
value_t compile_form(value_t expr, value_t next);

value_t compile(value_t expr, value_t next) {
	value_t result;

	if (is_symbol(expr)) {
		result = make_list(OP_LOOKUP, expr, next, 0);
	}
	else if (is_pair(expr)) {
		result = compile_form(expr, next);	
	}
	else if (expr == EMPTY_LIST) {
		error(1, 0, "Illegal empty combination ()");
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
		SYM_BEGIN = make_symbol("begin");
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
	else if (head == SYM_DEFINE) {
		result = compile_define(pair_right(expr), next);
	}
	// FIXME: This should be a macro. But we have no macro system so far.
	else if (head == SYM_BEGIN) {
		result = compile_begin(pair_right(expr), next);
	}
	else {
		result = compile_application(expr, next);
	}
	return result;
}


value_t compile_lambda(value_t expr, value_t next) {
	// TODO: Support implicit begin block inside lambda.
	int32_t arguments = pair_linked_length(expr);
	if (arguments < 2) {
		error(1, 0, "Expected at least 2 arguments for 'lambda', got %d", arguments);
	}
	
	value_t arg_list = pair_left(expr);
	value_t body = pair_right(expr);

	value_t compiled_body = compile_sequence(body, make_list(OP_RETURN, 0));

	return make_list(OP_CLOSURE, arg_list, compiled_body, next, 0);
}

// FIXME: This should be a macro. But we have no macro system so far.
value_t compile_begin(value_t expr, value_t next) {
	value_t lambda_form = make_pair(SYM_LAMBDA, make_pair(EMPTY_LIST, expr));
	value_t substitute = make_list(lambda_form, 0);
	return compile(substitute, next);
}
value_t compile_sequence(value_t expr_list, value_t next) {
	// The desired output of this is:
	//   compile_sequence( { exp1, exp2, exp3 }, next)
	//   -> compile(exp1, compile(exp2, compile(exp3, next)))
	value_t result;
	if (expr_list == EMPTY_LIST) {
		// We got to the bottom of the sequence, it should return simply the 
		// value given as "next", since there's nothing else to compile.
		result = next;
	}
	else {
		// Here we have a recursion to make. A stack consuming one, since we
		// want to compile them from the bottom to the top of the sequence, so
		// we can chain the compilation using the compiled next expression as 
		// 'next' value for this one.
		value_t expression = pair_left(expr_list);
		value_t expr_tail = pair_right(expr_list);
		value_t expr_next = compile_sequence(expr_tail, next);
		result = compile(expression, expr_next);
	}
	return result;
}

value_t compile_define(value_t args, value_t next) {
	int32_t arguments = pair_linked_length(args);
	if (arguments != 2) {
		error(1, 0, "Expected 2 arguments for 'define', got '%d'", arguments);
	}

	value_t symbol = pair_left(args);
	value_t expr = pair_left(pair_right(args));
	
	value_t bind = make_list(OP_BIND, symbol, next, 0);
	return compile(expr, bind);
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
	value_t quoted_obj = pair_left(expr);
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

	int n_args = 0;
	while (args != EMPTY_LIST) {
		n_args++;
		result = compile(pair_left(args), make_list(OP_ARGUMENT, result, 0));
		args = pair_right(args);
	}
	// Detecting tail calls.
	value_t next_op = pair_left(next);
	if (next_op != OP_RETURN) {
		// If we're not making a tail call, we should save the current frame
		// before doing all the function-application code generated above.
		result = make_list(OP_FRAME, next, result, 0);
	}

	return result;
}

