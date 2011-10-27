%{
#include <iostream>
#include <assert.h>

#include "scanner.h"

static token_t token_buf = { TK_NONE, "" };

%}


DIGIT                 [0-9]
WHITE                 [ \n\t]+
SIGN				  (\+|-)
IDENT_INITIAL           [-+a-zA-Z?!$%*&/:<=>^_~]
IDENT_SUBSEQUENT        {IDENT_INITIAL}|[0-9]

SYMBOL                  {IDENT_INITIAL}{IDENT_SUBSEQUENT}*
DECNUM                  {SIGN}?{DIGIT}+
BINNUM                  {SIGN}?[0-1]+
OCTNUM                  {SIGN}?[0-7]+
HEXNUM                  {SIGN}?({DIGIT}|[a-fA-F])+

%%


{WHITE}               /* Do Nothing */
;.*                   /* Do Nothing */
"."                   return TK_DOT;
"("                   return TK_LPAREN;
")"                   return TK_RPAREN;
"'"                   return TK_QUOTE;
"`"                   return TK_QUASIQUOTE;
",@"                  return TK_UNQUOTE_SPLICING;
","                   return TK_UNQUOTE;
\"(\\.|[^"])*\"       return TK_STRING;

"#d"?"#e"?{DECNUM}    return TK_INTEGER;
"#e"?"#d"?{DECNUM}    return TK_INTEGER;
"#x""#e"?{HEXNUM}     return TK_INTEGER;
"#e""#x"?{HEXNUM}     return TK_INTEGER;
"#o""#e"?{OCTNUM}     return TK_INTEGER;
"#e"?"#o"{OCTNUM}     return TK_INTEGER;
"#b""#e"?{BINNUM}     return TK_INTEGER;
"#e"?"#b"{BINNUM}     return TK_INTEGER;

{SYMBOL}              return TK_SYMBOL;

#[Ff]                 return TK_BOOLEAN_FALSE; 
#[Tt]                 return TK_BOOLEAN_TRUE;            

#[Uu]                 return TK_UNSPECIFIED;

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

