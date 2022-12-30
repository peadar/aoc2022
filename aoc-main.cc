#include "aoc.h"
#include <fstream>
#include <map>
#include <unistd.h>

int main(int argc, char *argv[]) {
    bool do_timeit = false;
    std::map<int, partfn> todo;
    for (int c; (c = getopt(argc, argv, "t12")) != -1; ) {
        switch (c) {
           case 't':
                do_timeit = true;
                break;
           case '1':
                todo[1] = part1;
                break;
           case '2':
                todo[2] = part2;
                break;

        }
    }
    if (todo.empty()){
       todo[1] = part1;
       todo[2] = part2;
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
    for (auto &[_, fn] : todo) {
       fn(in, std::cout);
       in.clear();
       in.seekg(0);
    }
}
