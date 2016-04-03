/*
*  Copyright (c) 2016 Ryan McCullagh <me@ryanmccullagh.com>
*
*  This program is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation, either version 3 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef INTERP_H
#define INTERP_H

#include <object.h>
#include <stdarg.h>
#include <setjmp.h>

#include "lexer.h"
#include "token.h"

typedef struct {
	jmp_buf 	cg_state;
	int 		cg_exception_status;
	char		*cg_exception_message;
} compiler_globals;

typedef struct {
	Lexer*  lexer;
	Object* symtab;
	Token current_token;
} Interpreter;

typedef enum InterpreterError {
	E_FATAL, E_NOTICE, E_WARNING, E_NO_ERROR, E_EOF
} InterpreterError;

extern void interp_init(Interpreter* self, Lexer* l);
extern void interp_eat(Interpreter* self, TokenType t);
extern long interp_expr(Interpreter* self);
extern long interp_term(Interpreter* self);
extern long interp_factor(Interpreter* self);
extern long interp_id(Interpreter* self);
extern void interp_error(Interpreter* self, InterpreterError type, const char* format, ...);
#endif
