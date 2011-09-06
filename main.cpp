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
#include "builtin_primitives.h"

value_t GLOBAL_ENVIRONMENT;

struct {
	std::string name;
	uint8_t arity;
	primitive_t func;
} 
primitives[] = {
	// BOOLEANS
	{ "and", 2, BP_and },
	{ "or", 2, BP_or },
	{ "not", 1, BP_not },
	// EQUIVALENCE
	{ "eq?", 2, BP_eqP },
	{ "eqv?", 2, BP_eqP },
	// PAIRS
	{ "cons", 2, BP_cons },
	{ "car", 1, BP_car },
	{ "cdr", 1, BP_cdr },
	// TOMBSTONE
	{ "", 0, 0 }
};


void bind_primitive(value_t env,
	                const std::string& name,
	                uint8_t arity,
	                primitive_t func) {
	primitive_descriptor_t *descriptor = new primitive_descriptor_t();
	descriptor->name = name;
	descriptor->arity = arity;

	value_t symbol = make_symbol(name);
	value_t primitive = make_primitive(func, descriptor);
	
	environment_add(env, symbol, primitive);
}

int main_repl(int argc, char ** argv) {
	value_t input;
	value_t result;
	std::cerr << "Welcome to Stutter." << std::endl;

	value_t global_env = make_environment(UNSPECIFIED);
	GLOBAL_ENVIRONMENT = global_env;

	environment_add(global_env, make_symbol("T"), BOOLEAN_TRUE);
	environment_add(global_env, make_symbol("F"), BOOLEAN_FALSE);
	
	// Register primitives into toplevel environment.
	for (int i = 0; primitives[i].func != 0; i++) {
		bind_primitive(global_env, primitives[i].name,
		                           primitives[i].arity,
		                           primitives[i].func);
	}
	
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
