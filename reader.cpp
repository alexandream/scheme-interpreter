#include <string>
#include <iostream>
#include <assert.h>
#include <error.h>

#include "reader.h"
#include "pair.h"
#include "scanner.h"
#include "fixnum.h"
#include "special.h"
#include "symbol.h"
#include "string.h"

static
value_t read_list(void);
static
value_t read_quote_shortcut(void);
static
value_t read_quasiquote_shortcut(void);
static
value_t read_unquote_splicing_shortcut(void);
static
value_t read_unquote_shortcut(void);

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
		case TK_INTEGER:
			get_token();
			result = fixnum_preprocess(input.lexeme);
			break;
		case TK_LPAREN:
			get_token(); // consume TK_LPAREN;
			// FIXME: Protect memory from collection.
			result = read_list();
			break;
		case TK_QUOTE:
			get_token(); // consume TK_QUOTE;
			result = read_quote_shortcut();
			break;
        case TK_QUASIQUOTE:
            get_token();
            result = read_quasiquote_shortcut();
            break;
        case TK_UNQUOTE_SPLICING:
            get_token();
            result = read_unquote_splicing_shortcut();
            break;
        case TK_UNQUOTE:
            get_token();
            result = read_unquote_shortcut();
            break;
		case TK_EOF:
			get_token(); // consume TK_EOF;
			result = END_OF_FILE;
			break;
	    case TK_STRING:
			get_token();
			result = string_preprocess(input.lexeme);
			break;
		case TK_SYMBOL:
			get_token();
			result = make_symbol(input.lexeme);
			break;
        
        case TK_UNSPECIFIED:
            get_token();
            result = UNSPECIFIED;
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
		protect_value(x);
		next = peek_token();
		if (next.type == TK_DOT) {
			get_token();
			value_t xs = read();
			protect_value(xs);
			next = get_token();
			assert(next.type == TK_RPAREN);
			result = make_pair(x, xs);
			unprotect_storage(2);
		}
		else {
			value_t y = read_list();
			protect_value(y);
			result = make_pair(x, y);
			unprotect_storage(2);
		}
	}
	return result;
}

static
value_t read_quote_shortcut(void) {
	value_t quote = make_symbol("quote");
	value_t next = read();

	protect_value(next);
	value_t result = make_list(quote, next, 0);
	unprotect_storage(1);
	return result;
}

static
value_t read_quasiquote_shortcut(void) {
	value_t quasiquote = make_symbol("quasiquote");
	value_t next = read();

	protect_value(next);
	value_t result = make_list(quasiquote, next, 0);
	unprotect_storage(1);
	return result;
}

static
value_t read_unquote_splicing_shortcut(void) {
	value_t unquote_splicing = make_symbol("unquote-splicing");
	value_t next = read();

	protect_value(next);
	value_t result = make_list(unquote_splicing, next, 0);
	unprotect_storage(1);
	return result;
}

static
value_t read_unquote_shortcut(void) {
	value_t unquote = make_symbol("unquote");
	value_t next = read();

	protect_value(next);
	value_t result = make_list(unquote, next, 0);
	unprotect_storage(1);
	return result;
}

