#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include "solve.h"
#include <unistd.h>

extern FILE *yyin;

namespace {

void part1() {
}

void part2() {
}
}

int main(int argc, char *argv[]) {
    for (int c; (c = getopt(argc, argv, "f:")) != -1; ) {
        switch (c) {
            case 'f':
                yyin = fopen(optarg, "r");
                assert(yyin);
                break;
        }
    }
    Network valves;
    yyparse(valves);

    std::cout << "graph { \n";
    for (auto &[name, valve] : valves) {
        for (auto &connected : valve.connected) {
            if (connected > &valve)
                std::cout << "\t" << name << "_" << valve.flowRate << " -- " << connected->name << "_" << connected->flowRate << "\n";
            else
                assert(connected->connected.find(&valve) != connected->connected.end()); // All links should be bidirectional
        }
    }
    std::cout << "}\n";
    part1();
    part2();
    return 0;
}
