#include "../aoc.h"
#include <unistd.h>
#include <iostream>
#include <cassert>

struct Lava { int location[3]; };

std::istream & operator >> (std::istream &in, Lava &l) {
    char _;
    in >> l.location[0] >> _ >> l.location[1] >> _ >> l.location[2];
    return in;
}

void part1(std::istream &is, std::ostream &os) {
    std::vector<Lava> lava;
    for (;;) {
        Lava l;
        is >> l;
        if (!is.good())
            break;
        lava.push_back(l);
    }
    size_t touching = 0;
    for (size_t i = 0; i < lava.size() - 1; ++i) {
        for (size_t j = i+1; j < lava.size(); ++j) {
            for (size_t k = 0; k < 3; ++k) {
                if (abs(lava[i].location[k] - lava[j].location[k]) == 1 &&
                        lava[i].location[(k+1) % 3] == lava[j].location[(k + 1) % 3] &&
                        lava[i].location[(k+2) % 3] == lava[j].location[(k + 2) % 3])
                    touching++;
            }
        }
    }
    os << "part1: " << lava.size() * 6 - touching * 2 << "\n";
}

/* Part 2 - flood-fill */
enum class Cell { AIR = 0, LAVA, STEAM };

template <int X, int Y, int Z> long visit(Cell (&space)[X][Y][Z], int x, int y, int z) {
    space[x][y][z] = Cell::STEAM;
    auto docell = [&space] (int x, int y, int z) -> long {
        if (x < 0 || x == X || y < 0 || y == Y || z < 0 || z == Z)
           return 0;
        switch (space[x][y][z]) {
           case Cell::LAVA: return 1;
           case Cell::AIR: return visit(space, x, y, z);
           case Cell::STEAM: return 0;
           default: abort(); return 0;
        }
    };
    return docell(x-1, y, z) + docell(x+1, y, z) + docell(x, y-1, z)
       + docell(x, y+1, z) + docell(x, y, z-1) + docell(x, y, z+1);
}

template <int X, int Y, int Z> void part2t(std::istream &is, std::ostream &os) {
    Cell space[X][Y][Z] {};
    for (;;) {
        int x, y, z;
        char _;
        is >> x >> _ >> y >> _ >> z;
        if (!is.good())
            break;
        // Add one to the dimensions so we can have an empty border around the data.
        x++, y++, z++;
        space[x][y][z] = Cell::LAVA;
    }
    // point 0, 0, 0 is part of our extended border, so it must be reachable
    os << "part2: " << visit(space, 0, 0, 0) << "\n";
}

void part2(std::istream &is, std::ostream &os) { part2t<24, 24, 24>(is, os); }
