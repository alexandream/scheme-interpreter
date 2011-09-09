%{
#include <iostream>
#include <assert.h>

#include "scanner.h"

static token_t token_buf = { TK_NONE, "" };

%}


DIGIT                 [0-9]
WHITE                 [ \n\t]+

IDENT_INITIAL           [-a-zA-Z?!$%*&/:<=>^_~]
IDENT_SUBSEQUENT        {IDENT_INITIAL}|[0-9]

SYMBOL                  {IDENT_INITIAL}{IDENT_SUBSEQUENT}*


%%


{WHITE}               /* Do Nothing */
"("                   return TK_LPAREN;
")"                   return TK_RPAREN;
"'"                   return TK_QUOTE;

-?{DIGIT}+            return TK_INTEGER;

{SYMBOL}              return TK_SYMBOL;

#[Ff]                 return TK_BOOLEAN_FALSE; 
#[Tt]                 return TK_BOOLEAN_TRUE;            

<<EOF>>               return TK_EOF;
.                     return TK_UNKNOWN;

%%

void scanner_push(FILE* fp) {
	yyin = fp;
	YY_BUFFER_STATE bufstate = yy_create_buffer(yyin, YY_BUF_SIZE);
	yypush_buffer_state(bufstate);
	
}


int scanner_pop() {
	yypop_buffer_state();
	if (!YY_CURRENT_BUFFER) {
		yyterminate();
	}
	return 0;
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

