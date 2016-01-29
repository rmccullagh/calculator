#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <mutablestring.h>
#include "token.h"

typedef struct Lexer {
	const char* text;
	size_t length;
	size_t position;
	char current_char;
} Lexer;

extern void lexer_init(Lexer* lexer, const char* text, size_t text_length);
extern void lexer_adv(Lexer* lexer);
extern void lexer_skip_white(Lexer* lexer);
extern void lexer_int(Lexer*);
extern MutableString* lexer_id(Lexer* self);
extern long lexer_number(Lexer*);
extern Token lexer_get_next_token(Lexer* self);

#endif
