%{
#include <iostream>
#include <assert.h>

#include "scanner.h"

static token_t token_buf = { TK_NONE, "" };

%}

DIGIT                 [0-9]
WHITE                 [ \n\t]+

%%

{WHITE}               /* Do Nothing */
{DIGIT}               return TK_INTEGER;
<<EOF>>               return TK_EOF;
%%

void scanner_init(FILE* fp) {
	static bool initialized = false;
	assert(!initialized);

	if (fp == NULL) {
		yyin = stdin;
	}
	else {
		yyin = fp;
	}
}

token_t peek_token() {
	if (token_buf.type == TK_NONE) {
		token_buf.type = (token_types_t) yylex();
		token_buf.lexeme = yytext;
	}
	return token_buf;
}

token_t get_token() {
	token_t result;
	if (token_buf.type != TK_NONE) {
		result = token_buf;
		token_buf.type = TK_NONE;
	}
	else {
		result.type = (token_types_t) yylex();
		result.lexeme = yytext;
	}
	return result;
}

