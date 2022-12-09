CXXFLAGS += -g --std=c++2a -Wall -Wextra -O3
.PHONY: all clean

all: solve

solve: solve.o
	$(CXX) -o $@ $^

check: all
	./solve ./input.txt
clean:
	rm -f *.o solve
