#include <iostream>
#include <sstream>
#include <stdio.h>

#include "value.h"

#include "reader.h"
#include "pair.h"
#include "evaluator.h"
#include "printer.h"
#include "special.h"
#include "symbol.h"
#include "scanner.h"
#include "storage.h"
#include "pointer.h"
#include "environment.h"
#include "primitive.h"
#include "boolean_primitives.h"

primitive_descriptor_t and_primitive_details = { "and", 2 };
primitive_descriptor_t or_primitive_details = { "or", 2 };
primitive_descriptor_t not_primitive_details = { "not", 2 };

int main_repl(int argc, char ** argv) {
	value_t input;
	value_t result;
	std::cerr << "Welcome to PFC." << std::endl;

	environment_t *global_env = make_environment(NULL);
	environment_set(global_env, make_symbol("T"), BOOLEAN_TRUE);
	environment_set(global_env, make_symbol("F"), BOOLEAN_FALSE);
	
	value_t and_symbol = make_symbol("and");
	value_t and_primitive = make_primitive(boolean_primitive_and,
	                                       &and_primitive_details);

	value_t or_symbol = make_symbol("or");
	value_t or_primitive = make_primitive(boolean_primitive_or,
	                                      &or_primitive_details);

	value_t not_symbol = make_symbol("not");
	value_t not_primitive = make_primitive(boolean_primitive_not,
	                                       &not_primitive_details);

	environment_set(global_env, and_symbol, and_primitive);
	environment_set(global_env, or_symbol, or_primitive);
	environment_set(global_env, not_symbol, not_primitive);

	while(true) {
		std::cerr << "> ";
		input = read();
		if (input == END_OF_FILE) {
			std::cout << std::endl;
			break;
		}
		result = evaluate(input, global_env);
		println(result);
	}
	return 0;
}

int main_symbol_test(int argc, char** argv) {
	value_t s1 = make_symbol("hello");
	std::cout << "0x" << std::hex << (uint64_t) s1 << ": " 
		      << symbol_format(s1) << std::endl;

	value_t s2 = make_symbol("hello");
	std::cout << "0x" << std::hex << (uint64_t) s2 << ": " 
		      << symbol_format(s2) << std::endl;

	value_t s3 = make_symbol("world");
	std::cout << "0x" << std::hex << (uint64_t) s3 << ": "
		      << symbol_format(s3) << std::endl;
	
	value_t s4 = make_symbol("worla");
	std::cout << "0x" << std::hex << (uint64_t) s4 << ": "
		      << symbol_format(s4) << std::endl;
	return 0;
}

int main_lexer_test(int argc, char** argv) {
	token_t input;
	do {
		input = get_token();
		std::cout << "Got token: " << input.type << " with lexeme: " 
		          << input.lexeme << std::endl;
	} while (input.type != TK_EOF);
	return 0;
}

int main_environment_test(int argc, char**argv) {
	environment_t *global = make_environment(NULL);
	value_t s1 = make_symbol("foo");
	value_t s2 = make_symbol("bar");
	value_t s3 = make_symbol("wakka");
	
	environment_set(global, s1, BOOLEAN_TRUE);
	println(environment_get(global, s1));

	environment_set(global, s2, BOOLEAN_FALSE);
	println(environment_get(global, s2));
	environment_set(global, s2, BOOLEAN_TRUE);
	println(environment_get(global, s2));
	println(environment_get(global, s3));
	return 0;
}

int main_allocator_test(int argc, char **argv) {
	double_storage_t* c1 = alloc_double_storage();
	std::cout << "Allocated a pair in address " << c1 << std::endl;
	double_storage_t* c2 = alloc_double_storage();
	std::cout << "Allocated a pair in address " << c2 << std::endl;
	return 0;
}

int main_printer_test(int argc, char**argv) {
	value_t c1 = make_pair(BOOLEAN_TRUE, EMPTY_LIST);
	println(c1);
	value_t c2 = make_pair(BOOLEAN_FALSE, c1);
	println(c2);
	value_t c3 = make_pair(BOOLEAN_TRUE, c2);
	println(c3);
	return 0;
}
int (*prog_pool[])(int, char**) = {
	main_repl,
	main_symbol_test,
	main_lexer_test,
	main_environment_test, //3
	main_allocator_test,
	main_printer_test
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
