#include <iostream>
#include <stdio.h>

#include "reader.h"
#include "evaluator.h"
#include "printer.h"
#include "special.h"


int main(int argc, char** argv) {
	value_t input;
	value_t result;
	std::cout << "Welcome to PFC." << std::endl;
	while(true) {
		std::cout << "> ";
		input = read();
		std::cerr << "DEBUG: " << input << std::endl;
		if (input == END_OF_FILE) {
			std::cout << std::endl;
			break;
		}
		result = evaluate(input);
		print(result);
	}
	return 0;
}
