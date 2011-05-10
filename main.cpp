#include <iostream>
#include <sstream>
#include <stdio.h>

#include "reader.h"
#include "evaluator.h"
#include "printer.h"
#include "special.h"
#include "symbol.h"
#include "scanner.h"



int main_repl(int argc, char ** argv) {
	value_t input;
	value_t result;
	std::cerr << "Welcome to PFC." << std::endl;
	while(true) {
		std::cerr << "> ";
		input = read();
		if (input == END_OF_FILE) {
			std::cout << std::endl;
			break;
		}
		result = evaluate(input);
		print(result);
	}
	return 0;
}

int main_symbol_test(int argc, char** argv) {
	value_t s1 = symbol_make_from_string("hello");
	std::cout << "0x" << std::hex << (uint64_t) s1 << ": " 
		      << symbol_format(s1) << std::endl;

	value_t s2 = symbol_make_from_string("hello");
	std::cout << "0x" << std::hex << (uint64_t) s2 << ": " 
		      << symbol_format(s2) << std::endl;

	value_t s3 = symbol_make_from_string("world");
	std::cout << "0x" << std::hex << (uint64_t) s3 << ": "
		      << symbol_format(s3) << std::endl;
	
	value_t s4 = symbol_make_from_string("worla");
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

int (*prog_pool[])(int, char**) = {
	main_repl,
	main_symbol_test,
	main_lexer_test
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
