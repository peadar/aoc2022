OPT ?= -O0 -pipe
CXXFLAGS += -g --std=c++2a -Wall -Wextra $(OPT) -I..

.PHONY: all clean local-clean
VPATH = ..

all: solve

OBJS += solve.o aoc-main.o timeit.o

solve: $(OBJS)
	$(CXX) -o $@ $^

check: all
	./solve ./input.txt

clean: local-clean
	rm -f *.o solve
