#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interp.h"
#include "lexer.h"

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
	
	return i;
}

int main(int argc, char** argv)
{

	while(1) {
		char buffer[80];

		size_t bytes_read = raw_input("calc> ", buffer, sizeof(buffer));
		if(!bytes_read) {
			printf("\n");
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
	
	return 0;
}

