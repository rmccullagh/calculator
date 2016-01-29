objects = main.o lexer.o token.o interp.o
CCFLAGS = -Wall -Wextra
LDFLAGS = -lmutablestring -lobject

main : $(objects)
	cc $(CCFLAGS) -o main $(objects) $(LDFLAGS)
.PHONY : clean
clean:
	rm -f main $(objects)
