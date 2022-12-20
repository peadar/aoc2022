CXXFLAGS = -g -O0 --std=c++17 -fno-omit-frame-pointer

test: all
	@./solve < input.txt

all: solve
solve: lexer.o parser.o solve.o
	$(CXX) -o $@ $^
lexer.cc: lexer.l parser.h
	flex -o $@ lexer.l
parser.cc parser.h: parser.y
	bison --debug -o parser.cc --defines=parser.h $^
clean:
	rm -rf *.o solve parser.cc parser.h lexer.cc parser.tab.c parser.output
