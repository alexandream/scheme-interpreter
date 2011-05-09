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
		case TK_BOOLEAN_TRUE:
			get_token();
			result = BOOLEAN_TRUE;
			break;
		
		case TK_BOOLEAN_FALSE:
			get_token();
			result = BOOLEAN_FALSE;
			break;

		case TK_EOF:
			result = END_OF_FILE;
			break;

		default:
			error(1, 0, "Unknown token with lexeme: %s\n",
				  input.lexeme.c_str());
	}
	return result;
	
}


