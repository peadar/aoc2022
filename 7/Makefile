CXXFLAGS += -g -O0 --std=c++17 -I..
.PHONY: all clean

all: solve

solve: solve.o
	$(CXX) -o $@ $^

check: all
	./solve < ./input.txt
clean:
	rm -f *.o solve
