OPT ?= -O3
CXXFLAGS += -g --std=c++2a -Wall -Wextra $(OPT) -I..

.PHONY: all clean
VPATH = ..

all: solve

solve.s: solve.cc
	$(CXX) -S -o $@ $^

solve: solve.o aoc-main.o timeit.o
	$(CXX) -o $@ $^

check: all
	./solve ./input.txt
clean:
	rm -f *.o solve
