#include <stdio.h>
#include <stdlib.h>
#include <object.h>
#include <limits.h>
#include <stdarg.h>
#include <string.h>
#include <setjmp.h>

#include "interp.h"
#include "lexer.h"

extern compiler_globals cg;

static void init_predefined_symbols(Interpreter* self)
{
	Object* long_max = newLong(LONG_MAX);
	mapInsert(self->symtab, "LONG_MAX", long_max);
	objectDestroy(long_max);
	Object* long_min = newLong(LONG_MIN);
	mapInsert(self->symtab, "LONG_MIN", long_min);
	objectDestroy(long_min);
	Object* int_max = newLong(INT_MAX);
	mapInsert(self->symtab, "INT_MAX", int_max);
	objectDestroy(int_max);
	Object* int_min = newLong(INT_MIN);
	mapInsert(self->symtab, "INT_MIN", int_min);
	objectDestroy(int_min);
}

void interp_init(Interpreter* self, Lexer* lexer)
{
	self->lexer = lexer;
	self->symtab = newMap(1);
	self->current_token = lexer_get_next_token(self->lexer);
	init_predefined_symbols(self);
}

void interp_error(Interpreter* self, InterpreterError type, const char* format, ...)
{
	va_list ap;
	char buffer[1024];
	int n = 0;

	va_start(ap, format);
	n = vsnprintf(buffer, 1024, format, ap);
	va_end(ap);

	if(cg.cg_exception_message == NULL) {
		cg.cg_exception_message = malloc(strlen(buffer) + 1);
		memcpy(cg.cg_exception_message, buffer, strlen(buffer) + 1);
	} else {
		free(cg.cg_exception_message);
		cg.cg_exception_message = NULL;
		cg.cg_exception_message = malloc(strlen(buffer) + 1);
		memcpy(cg.cg_exception_message, buffer, strlen(buffer) + 1);
	}

	cg.cg_exception_status = type;

	longjmp(cg.cg_state, 1);
}

void interp_eat(Interpreter* self, TokenType t)
{
	if(self->current_token.type == t) {
		self->current_token = lexer_get_next_token(self->lexer);
	} else {
		interp_error(self, E_FATAL, "unexpected token %s, expected %s", 
			token_str(self->current_token.type), token_str(t));
	}
}

long interp_id(Interpreter* self)
{
	Token t = self->current_token;
	interp_eat(self, T_ID);
	
	const char* name = t.u1.id->buffer;

	if(strcmp(name, "exit") == 0) {
		cg.cg_exception_message = "Success";
		cg.cg_exception_status = E_NO_ERROR;
		longjmp(cg.cg_state, 1);
	}

	Object* value = mapSearch(self->symtab, name);

	if(!value) {
		interp_error(self, E_WARNING, "undefined symbol \"%s\"", name); 
		return 0;	
	} else {
		if(O_TYPE(value) == IS_LONG) {
			long retval = O_LVAL(value);
			objectDestroy(value);
			return retval;
		} else {
			interp_error(self, E_NOTICE, "value type is not supported");
			objectDestroy(value);
			return 0;
		}
	}
}

long interp_factor(Interpreter* self)
{
	Token t = self->current_token;
	
	if(t.type == T_INT) {
		interp_eat(self, T_INT);
		return t.u1.long_value;
	} else if(t.type == T_ID) {
		return interp_id(self);	
	} else {
		interp_error(self, E_FATAL, "unexpected token %s", token_str(t.type));
		/* NOT REACHED */
		return -LONG_MAX;
	}
}

long interp_term(Interpreter* self)
{
	long result = interp_factor(self);

	while(self->current_token.type == T_MUL || self->current_token.type == T_DIV) {
		Token t = self->current_token;
		if(t.type == T_MUL) {
			interp_eat(self, T_MUL);
			result = result * interp_factor(self);
		} else if(t.type == T_DIV) {
			interp_eat(self, T_DIV);
			long divisor = interp_factor(self);
			if(divisor == 0) {
				interp_error(self, E_WARNING, "Divison by zero is undefined");	
				return 0;
			}
			result = result / divisor;
		}
	}

	return result;
}

long interp_expr(Interpreter* self)
{
	long result = interp_term(self);

	while(self->current_token.type == T_PLUS || self->current_token.type == T_MINUS) {
		Token t = self->current_token;
		if(t.type == T_PLUS) {
			interp_eat(self, T_PLUS);
			result = result + interp_term(self);
		} else if(t.type == T_MINUS) {
			interp_eat(self, T_MINUS);
			result = result - interp_term(self);
		}
	}

	return result;
}



