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

#include <stdlib.h>
#include <string.h>
#include <mutablestring.h>

#include "lexer.h"
#include "token.h"

void lexer_init(Lexer* lexer, const char* text, size_t text_length)
{
	lexer->text = text;
	lexer->length = text_length;
	lexer->position = 0;
	lexer->current_char = lexer->text[lexer->position];
}

void lexer_adv(Lexer* lexer)
{
	lexer->position += 1;

	if(lexer->position == lexer->length) {
		lexer->current_char = '\0';
	} else {
		lexer->current_char = lexer->text[lexer->position];
	}
}

static int is_white(char c)
{
	return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

void lexer_skip_white(Lexer* self)
{
	while(self->current_char != '\0' && is_white(self->current_char))
		lexer_adv(self);
}

static int is_id(char c) {
	return ((c <= 'Z' && c >= 'A') || (c <= 'z' && c >= 'a')) || c == '_';
}

static int is_digit(char c) {
	return c <= '9' && c >= '0';
}

MutableString* lexer_id(Lexer* self)
{
	MutableString* m = mutable_string_new(NULL);

	while(self->current_char != '\0' && is_id(self->current_char)) {
		mutable_string_append_c(m, self->current_char);
		lexer_adv(self);
	}
	
	return m;
}

long lexer_number(Lexer* self)
{
	MutableString* m = mutable_string_new_len(sizeof("-18446744073709551615") - 1);

	while(self->current_char != '\0' && is_digit(self->current_char)) {
		mutable_string_append_c(m, self->current_char);
		lexer_adv(self);
	}

	long retval = strtol(m->buffer, NULL, 10);
	mutable_string_free(m);
	return retval;
}

Token lexer_get_next_token(Lexer* self)
{

	while(self->current_char != '\0') {
		if(is_white(self->current_char)) {
			lexer_skip_white(self);
			continue;
		}
		if(is_id(self->current_char)) {
			Token t;
			token_init(&t, T_ID);
			MutableString* id = lexer_id(self);
			t.u1.id = id;
			return t;
		}
		if(is_digit(self->current_char)) {
			Token t;
			token_init(&t, T_INT);
			t.u1.long_value = lexer_number(self);
			return t;
		}
		if(self->current_char == '+') {
			lexer_adv(self);
			Token t;
			token_init(&t, T_PLUS);
			t.u1.char_value = '+';
			return t;
		}
		if(self->current_char == '-') {
			lexer_adv(self);
			Token t;
			token_init(&t, T_MINUS);
			t.u1.char_value = '-';
			return t;
		}
		if(self->current_char == '*') {
			lexer_adv(self);
			Token t;
			token_init(&t, T_MUL);
			t.u1.char_value = '*';
			return t;
		}
		if(self->current_char == '/') {
			lexer_adv(self);
			Token t;
			token_init(&t, T_DIV);
			t.u1.char_value = '/';
			return t;
		}
		
		printf("invalid input\n");
		
		Token t;
		token_init(&t, T_EOF);
		t.u1.char_value = '\0';
		return t;
	}
}

