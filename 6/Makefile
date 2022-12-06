CXXFLAGS += -g
.PHONY: all clean

all: solve

solve: solve.o
	$(CXX) -o $@ $^

check: all
	./solve ./input.txt
clean:
	rm -f *.o solve
