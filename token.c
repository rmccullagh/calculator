#include <stdlib.h>
#include "token.h"

static const char* const token_symbol[] = {
	"T_ID", "T_INT", "T_PLUS", "T_MINUS", "T_MUL", "T_DIV", "T_EOF"
};

void token_init(Token* self, TokenType t)
{
	self->type = t;
}

const char* token_str(TokenType t)
{
	return token_symbol[t];
}


