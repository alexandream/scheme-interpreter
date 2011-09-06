#include <string>
#include <iostream>
#include <assert.h>
#include <error.h>

#include "reader.h"
#include "pair.h"
#include "scanner.h"
#include "special.h"
#include "symbol.h"

static
value_t read_list(void);
static
value_t read_quote_shortcut(void);

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
		
		case TK_LPAREN:
			get_token(); // consume TK_LPAREN;
			result = read_list();
			break;
		case TK_QUOTE:
			get_token(); // consume TK_QUOTE;
			result = read_quote_shortcut();
			break;
		case TK_EOF:
			result = END_OF_FILE;
			break;
		
		case TK_SYMBOL:
			get_token();
			result = make_symbol(input.lexeme);
			break;

		default:
			error(1, 0, "Unknown token with lexeme: %s\n",
				  input.lexeme.c_str());
	}
	return result;
	
}

static
value_t read_list(void) {
	token_t next = peek_token();
	value_t result;
	if (next.type == TK_RPAREN) {
		get_token();
		result = EMPTY_LIST;
	}
	else {
		// Don't consume anything. read() will take care of it.
		value_t x = read();
		value_t y = read_list();
		result = make_pair(x, y);
	}
	return result;
}

static
value_t read_quote_shortcut(void) {
	value_t quote = make_symbol("quote");
	value_t next = read();
	value_t result = make_list(quote, next, 0);
	return result;
}
