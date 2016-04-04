/*
 * Copyright (c) 2016 Ryan McCullagh <me@ryanmccullagh.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
