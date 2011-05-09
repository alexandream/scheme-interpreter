#include <string>
#include <iostream>
#include <assert.h>
#include <error.h>

#include "reader.h"
#include "scanner.h"
#include "special.h"

value_t read(void) {
	value_t result;
	token_t input;
	input = peek_token();
	// TODO: Swap this switch for a dynamic dispatching schema later?
	switch (input.type) {
		case TK_EOF:
			result = END_OF_FILE;
			break;
		default:
			error(1, 0, "Unknown token type: %d with lexeme: %s\n",
				  input.type,
				  input.lexeme.c_str());
	}
	return result;
	
}


