#include <iostream>
#include <sstream>
#include <stdio.h>

#include "value.h"

#include "reader.h"
#include "pair.h"
#include "evaluator.h"
#include "compiler.h"
#include "printer.h"
#include "formatter.h"
#include "special.h"
#include "symbol.h"
#include "scanner.h"
#include "storage.h"
#include "pointer.h"
#include "environment.h"
#include "primitive.h"
#include "boolean_primitives.h"
#include "builtin_primitives.h"

value_t GLOBAL_ENVIRONMENT;

primitive_descriptor_t and_primitive_details = { "and", 2 };
primitive_descriptor_t or_primitive_details = { "or", 2 };
primitive_descriptor_t not_primitive_details = { "not", 1 };
primitive_descriptor_t eqP_primitive_details = { "eq?", 2 };
primitive_descriptor_t eqvP_primitive_details = { "eqv?", 2 };

int main_repl(int argc, char ** argv) {
	value_t input;
	value_t result;
	std::cerr << "Welcome to Stutter." << std::endl;

	value_t global_env = make_environment(UNSPECIFIED);
	GLOBAL_ENVIRONMENT = global_env;

	environment_add(global_env, make_symbol("T"), BOOLEAN_TRUE);
	environment_add(global_env, make_symbol("F"), BOOLEAN_FALSE);
	
	value_t and_symbol = make_symbol("and");
	value_t and_primitive = make_primitive(boolean_primitive_and,
	                                       &and_primitive_details);

	value_t or_symbol = make_symbol("or");
	value_t or_primitive = make_primitive(boolean_primitive_or,
	                                      &or_primitive_details);

	value_t not_symbol = make_symbol("not");
	value_t not_primitive = make_primitive(boolean_primitive_not,
	                                       &not_primitive_details);

	value_t eqP_symbol = make_symbol("eq?");
	value_t eqP_primitive = make_primitive(BP_eqP,
	                                       &eqP_primitive_details);

	value_t eqvP_symbol = make_symbol("eqv?");
	value_t eqvP_primitive = make_primitive(BP_eqP,
	                                       &eqvP_primitive_details);
	
	environment_add(global_env, eqP_symbol, eqP_primitive);
	environment_add(global_env, eqvP_symbol, eqvP_primitive);
	environment_add(global_env, and_symbol, and_primitive);
	environment_add(global_env, or_symbol, or_primitive);
	environment_add(global_env, not_symbol, not_primitive);
	
	context_t* context = new context_t();
	context->accumulator = UNSPECIFIED;
	context->environment = global_env;
	context->value_stack = EMPTY_LIST;
	context->frame_stack = EMPTY_LIST;
	while(true) {
		std::cerr << "> ";
		input = read();
		if (input == END_OF_FILE) {
			std::cout << std::endl;
			break;
		}
		context->next_expr = compile(input, make_list(OP_HALT, 0));
#ifdef DEBUG_COMPILER
		std::cerr << "; " << format(context->next_expr) << std::endl;
#endif
		result = evaluate(context);
		std::cerr << ":::" << std::endl;
		println(result);
	}
	return 0;
}

int (*prog_pool[])(int, char**) = {
	main_repl
};

int main(int argc, char** argv) {
	int index = 0;

	if (argc > 1) {
		int i;
		std::string arg(argv[1]);
		std::stringstream stream(arg);
		if (stream >> i) {
			index = i;
		}
	}
	return prog_pool[index](argc, argv);
}
