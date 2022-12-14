#include "aoc.h"
#include <fstream>
#include <unistd.h>

int main(int argc, char *argv[]) {
    bool do_timeit = false;
    for (int c; (c = getopt(argc, argv, "t")) != -1; ) {
        switch (c) {
            case 't':
                do_timeit = true;
        }
    }

    std::fstream in(argv[optind]);
    if (do_timeit) {
        std::fstream null("/dev/null");
        timeit([&in, &null] {
                part2(in, null);
                in.clear();
                in.seekg(0);
                });
        return 0;
    }

    part1(in, std::cout);
    in.clear();
    in.seekg(0);
    part2(in, std::cout);
}
