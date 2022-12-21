
#include "aoc.h"
#include <cassert>
#include <unistd.h>


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
    std::vector<std::array<char, 7>> grid;
    void dropShape(const RockShape &shape, std::istream &is);
    bool collide(const RockShape &shape, int x, int y);
    void stamp(const RockShape &shape, int x, int y, Type);

    void draw(const char *msg, const RockShape *rs, int x, int y);
};


void Chamber::draw(const char *msg, const RockShape *rs, int rockx, int rocky) {
    return;
    std::cout << "\n" << msg << "\n";
    int y = grid.size() - 1;
    if (rs)
        y = std::max(rocky, y);
    for (; y >= 0; --y) {
        std::cout << "|";
        for (int x = 0; x < 7; ++x) {
            char c = y < int(grid.size()) ? grid[y][x] : AIR;
            if (rs) {
                for (auto p : rs->matter)
                    if (p.x == x - rockx && p.y == rocky - y)
                        c = FALLING;
            }
            std::cout << c;
        }
        std::cout << "\n";
    }
    std::cout << "+-------+\n\n";
    std::string in;
    std::getline(std::cin, in);
}

bool Chamber::collide(const RockShape &shape, int x, int y) {
    for (auto &point : shape.matter) {
        int ypos = y - point.y;
        int xpos = x + point.x;
        if (xpos < 0 || xpos >= 7)
            return true;
        if (ypos < 0 || ypos < grid.size() && grid[ypos][x + point.x] != AIR)
            return true;
    }
    return false;
}

void Chamber::stamp(const RockShape &shape, int x, int y, Type type) {
    for (auto &point : shape.matter) {
        int yloc = y - point.y;
        while (yloc >= (int)grid.size()) {
            grid.emplace_back();
            for (int i = 0; i < 7; ++i)
                grid.back()[i] = AIR;
        }
        char &c = grid[yloc][x + point.x];
        c = type;
    }
}


void
Chamber::dropShape(const RockShape &shape, std::istream &jets) {
    int y = grid.size() + 3 + shape.height - 1;
    int x = 2;
    draw("rock begins falling", &shape, x, y);
    for (;;) {
        char c = jets.get();
        getppid();

        if (jets.eof()) {
            jets.clear();
            jets.seekg(0);
            continue;
        }

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
        draw("rock moves with jet", &shape, x, y);
        if (y == 0 || collide(shape, x, y - 1))
            break;
        --y;
        draw("rock moves down", &shape, x, y);
    }
    stamp(shape, x, y, ROCK);
    draw("rock landed", nullptr, 0, 0);
}

void
part1(std::istream &in, std::ostream &out) {
    Chamber chamber;

    int count = 0;
    while (count < 2022) {
    for (auto &shape : shapes) {
        chamber.dropShape(shape, in);
        if (++count == 2022)
            break;
    }
    }
    std::cout << chamber.grid.size() << std::endl;
}

void
part2(std::istream &in, std::ostream &out) {
}
