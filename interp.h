#ifndef INTERP_H
#define INTERP_H

#include <object.h>
#include <stdarg.h>

#include "lexer.h"
#include "token.h"

typedef struct {
	Lexer*  lexer;
	Object* symtab;
	Token current_token;
} Interpreter;

typedef enum InterpreterError {
	E_FATAL, E_NOTICE, E_WARNING
} InterpreterError;

extern void interp_init(Interpreter* self, Lexer* l);
extern void interp_eat(Interpreter* self, TokenType t);
extern long interp_expr(Interpreter* self);
extern long interp_term(Interpreter* self);
extern long interp_factor(Interpreter* self);
extern long interp_id(Interpreter* self);
extern void interp_error(Interpreter* self, InterpreterError type, const char* format, ...);
#endif
