#include "aoc.h"
#include <fstream>

int main(int, char *argv[]) {
    std::fstream in(argv[1]);
    part1(in, std::cout);
    in.clear();
    in.seekg(0);
    part2(in, std::cout);

    std::fstream null("/dev/null");
    timeit([&in, &null] {
            in.clear();
            in.seekg(0);
            part2(in, null);
            });

}
