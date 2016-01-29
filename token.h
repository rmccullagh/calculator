#ifndef TOKEN_H
#define TOKEN_H

#include <mutablestring.h>

typedef enum {
	T_ID, T_INT, T_PLUS, T_MINUS, T_MUL, T_DIV, T_EOF
} TokenType;

typedef struct {
	TokenType type;
	union {
		long long_value;
		char char_value;
		MutableString* id;
	} u1;
} Token;

extern void token_init(Token*, TokenType);
extern const char* token_str(TokenType t);

#endif
