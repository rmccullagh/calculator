#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#include "interp.h"
#include "lexer.h"

compiler_globals cg = {0};

static int raw_input(const char* prompt, char* buffer, size_t buffer_size)
{
	size_t i = 0;
	int c;
	memset(buffer, '\0', buffer_size);
	printf("%s", prompt);
	
	while((c = getchar()) != EOF) {
		if(i < buffer_size - 1) {
			if((char)c == '\n') {
				break;
			} else {
				buffer[i++] = (char)c;
			}
		} else {
			int cc;
			while((cc = getchar()) != EOF && cc != '\n');
			break;
		}
	}

	buffer[i] = '\0';


	if(c == EOF) {
		cg.cg_exception_status = E_EOF;
		if(cg.cg_exception_message == NULL) {
			cg.cg_exception_message = malloc(sizeof("End of file"));
			memcpy(cg.cg_exception_message, "End of file", sizeof("End of file"));
		} else {
			free(cg.cg_exception_message);
			cg.cg_exception_message = NULL;
			cg.cg_exception_message = malloc(sizeof("End of file"));
			memcpy(cg.cg_exception_message, "End of file", sizeof("End of file"));
		}

		longjmp(cg.cg_state, 1);
	}
	
	return i;
}

int main(int argc, char** argv)
{
	cg.cg_exception_message = NULL;

	Event_Loop:

	if(setjmp(cg.cg_state)) {
		int retval;

		switch(cg.cg_exception_status) {
			case E_NOTICE:
				fprintf(stderr, "Notice: %s\n", cg.cg_exception_message);
			break;
			case E_WARNING:
				fprintf(stderr, "Warning: %s\n", cg.cg_exception_message);
			break;
			case E_FATAL:
				fprintf(stderr, "Fatal: %s\n", cg.cg_exception_message);
				retval = 1;
				goto Safe_Exit;
			break;
			case E_NO_ERROR:
				fprintf(stderr, "%s\n", cg.cg_exception_message);
				retval = 0;
				goto Safe_Exit;
			break;
			case E_EOF:
				fprintf(stderr, "\n%s\n", cg.cg_exception_message);
				retval = 0;
				goto Safe_Exit;
			default :
				fprintf(stderr, "%s\n", cg.cg_exception_message);
			break;	
		}
		
		if(cg.cg_exception_message != NULL) {
			free(cg.cg_exception_message);
			cg.cg_exception_message = NULL;
		}	
		
		goto Event_Loop;

		Safe_Exit:
			return retval;
	} else {
		while(1) {
			char buffer[80];
			size_t bytes_read = raw_input("calc> ", buffer, sizeof(buffer));
			if(!bytes_read) {
				continue;
			} else {	
				Lexer lex;
				lexer_init(&lex, buffer, bytes_read);	
				Interpreter interpreter;
				interp_init(&interpreter, &lex);
				long result = interp_expr(&interpreter);
				printf("%ld\n", result);
			}
		}
	}

	return 0;
}
