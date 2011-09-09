#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <string>

enum token_types_t {
   TK_NONE,
   TK_UNKNOWN,
   TK_EOF,
  
   TK_LPAREN,
   TK_RPAREN,
   TK_QUOTE, 

   TK_INTEGER,
   TK_SYMBOL,
   TK_BOOLEAN_FALSE,
   TK_BOOLEAN_TRUE,

   TK_DOT
};

struct token_t {
    token_types_t type;
	std::string lexeme;
};

token_t peek_token();
token_t get_token();
void scanner_push(FILE* fp);
int scanner_pop();

#endif

