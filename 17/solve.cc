#include "aoc.h"
#include <map>
#include <cassert>
#include <unistd.h>
#include <string.h>

struct Point {
    int x, y;
    Point(int x, int y) : x(x), y(y) {}
    Point(const Point &) = default;
    Point() = default;
};

struct RockShape {
    std::vector<Point> matter;
    int height;
    int width;
    RockShape(const std::vector<Point> &matter_) : matter(std::move(matter_)) {
        for (auto &point : matter) {
            if (point.x + 1> width)
                width  = point.x + 1;
            if (point.y + 1> height)
                height  = point.y + 1;
        }
    }
};

const RockShape shapes[] = {
    { { { 0, 0 }, { 1, 0}, {2, 0}, {3, 0} } } , // "####"
    { { { 1, 0 }, {0, 1}, {1,1}, {2,1}, {1,2} } }, // "+"
    { { { 2, 0 }, {2, 1}, {0,2}, {1,2}, {2,2} } }, // "backwards-L"
    { { { 0, 0 }, { 0, 1}, { 0, 2 }, { 0, 3 } } }, // I
    { { { 0, 0 }, { 1, 0}, { 0, 1 }, { 1, 1 } } }, // square
};

enum Type : char {
    AIR = '.',
    ROCK = '#',
    FALLING = '@'
};

struct Chamber {
    std::vector<uint8_t> grid;
    void dropShape(const RockShape &shape, const std::string jets, size_t &curoff);
    bool collide(const RockShape &shape, int x, int y);
    void stamp(const RockShape &shape, int x, int y);
};

bool Chamber::collide(const RockShape &shape, int x, int y) {
    for (auto &point : shape.matter) {
        int ypos = y - point.y;
        int xpos = x + point.x;
        if (xpos < 0 || xpos >= 7)
            return true;
        if (ypos < 0 || (ypos < int(grid.size()) && (grid[ypos] & (1 << (x + point.x)))))
            return true;
    }
    return false;
}

void Chamber::stamp(const RockShape &shape, int x, int y) {
    for (auto &point : shape.matter) {
        int yloc = y - point.y;
        while (yloc >= (int)grid.size())
            grid.push_back(0);
        grid[yloc] |= 1 << (x + point.x);
    }
}

void
Chamber::dropShape(const RockShape &shape, const std::string jets, size_t &curoff) {
    int y = grid.size() + 3 + shape.height - 1;
    int x = 2;

    for (;;) {
        char c = jets[curoff % jets.size()];
        curoff++;

        int newx;
        switch (c) {
            case '<':
                newx = x - 1;
                break;
            case '>':
                newx = x + 1;
                break;
            case '\n':
                continue;
            default:
                abort();
        }
        if (!collide(shape, newx, y))
            x = newx;
        if (y == 0 || collide(shape, x, y - 1))
            break;
        --y;
    }
    stamp(shape, x, y);
}

void
part1(std::istream &in, std::ostream &out) {
    Chamber chamber;

    std::string intext;
    in >> intext;
    size_t curoff = 0;
    int count = 0;
    while (count < 2022) {
        for (auto &shape : shapes) {
            chamber.dropShape(shape, intext, curoff);
            if (++count == 2022)
                break;
        }
    }
    out << "part1: " << chamber.grid.size() << std::endl;
}

void
part2(std::istream &in, std::ostream &out) {
    Chamber chamber;

    std::string intext;
    in >> intext;
    size_t curoff = 0;

    size_t lastHeight = 0;
    std::vector<uint8_t> turn2height; // won't extend more than 5 for each turn

    constexpr size_t measuringPoint = 50'000; // allow some iterations to let things stabilize
    constexpr size_t targetCount = 1'000'000'000'000;
    constexpr size_t checkCount = 1000; // check for a reapeating pattern this long.

    turn2height.reserve(measuringPoint);

    for (;;) {
        for (auto &shape : shapes) {
            chamber.dropShape(shape, intext, curoff);
            turn2height.push_back(chamber.grid.size() - lastHeight);
            lastHeight = chamber.grid.size();
            if (turn2height.size() == measuringPoint) {
                // find a point where 1000 items repeat
                auto base = turn2height.data() + turn2height.size() - checkCount;
                auto cmp = turn2height.data() + turn2height.size() - checkCount - 1;
                for (; cmp >= turn2height.data(); cmp--) {
                    if (memcmp(base, cmp, checkCount) == 0) {
                        size_t period = base - cmp;
                        /* reconstitute the height difference across the entire range */
                        size_t periodHeight = 0;
                        for (size_t i = 0; i < period; ++i)
                            periodHeight += turn2height[turn2height.size() - period + i];
                        auto remainingPeriods = (targetCount - measuringPoint) / period;
                        auto residuePeriod = (targetCount - measuringPoint) % period;
                        size_t residueHeight = 0;
                        for (size_t i = 0; i < residuePeriod; ++i)
                            residueHeight += turn2height[turn2height.size() - period + i];
                        out << "part2: " << chamber.grid.size() + remainingPeriods * periodHeight + residueHeight << "\n";
                        return;
                    }
                }
            }
        }
    }
}
