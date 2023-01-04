OBJS += lexer.o parser.o

include ../Makefile.inc

lexer.cc: lexer.l parser.h
	flex -o $@ lexer.l
parser.cc parser.h: parser.y
	bison --debug -o parser.cc --defines=parser.h $^


local-clean:
	rm -f lexer.cc parser.cc parser.h lexer.h
