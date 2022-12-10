CXXFLAGS += -g --std=c++2a -Wall -Wextra -O0
.PHONY: all clean

all: solve

solve.s: solve.cc
	$(CXX) -S -o $@ $^
solve: solve.o
	$(CXX) -o $@ $^

check: all
	./solve ./input.txt
clean:
	rm -f *.o solve
