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
