#include "aoc.h"
#include <deque>
#include <unistd.h>
#include <sstream>
#include <iostream>
#include <numeric>
#include <optional>
#include <cassert>
#include <limits>
using namespace std;

namespace {

using Point = std::pair<int, int>;
enum CellState : char { AIR = '.', ROCK = '#', SAND = 'o' };
constexpr int MIN_INT = std::numeric_limits<int>::min();
constexpr int MAX_INT = std::numeric_limits<int>::max();
template <int MinX, int MaxX,int MinY, int MaxY, bool stickyFloor> struct Grid;
template <int MinX, int MaxX, int MinY, int MaxY, bool stickyFloor> std::ostream &operator << (std::ostream &os, const Grid<MinX, MaxX, MinY, MaxY, stickyFloor> &g);

template <int MinX, int MaxX,int MinY, int MaxY, bool stickyFloor>
class Grid {
    static constexpr size_t SizeX = MaxX - MinX + 1;
    static constexpr size_t SizeY = MaxY - MinY + 1;
    std::vector<char> storage;
    mutable int maxx = MIN_INT, minx = MAX_INT, maxy = MIN_INT, miny = MAX_INT;
    size_t point2idx(const Point &p) const {
        // use x == 0 and x==X+1 for edges. Map actual x to x + 1, and width is X + 2
        auto [x, y] = p;
        assert(x >= MinX && x <= MaxX);
        assert(y >= MinY && y <= MaxY);
        maxx = std::max(maxx, x);
        maxy = std::max(maxy, y);
        minx = std::min(minx, x);
        miny = std::min(miny, y);
        return (y + MinY) * SizeX + x + MinX;
    }
    char &operator[](const Point &p) { return storage[point2idx(p)]; }
    template <int, int, int, int, bool> friend std::ostream & operator << (std::ostream &os, const Grid<MinX, MaxX, MinY, MaxY, stickyFloor> &g);
public:
    int floor;
    Grid(std::istream &is);
    Point sand();
};

template <int MinX, int MaxX, int MinY, int MaxY, bool stickyFloor>
std::ostream &operator << (std::ostream &os, const Grid<MinX, MaxX, MinY, MaxY, stickyFloor> &g) {
    for (int y = g.miny; y <= g.maxy; ++y) {
        for (int x = g.minx; x <= g.maxx; ++x)
            os << g[Point{x, y}];
        os << "\n";
    }
    return os;
}

template <int MinX, int MaxX, int MinY, int MaxY, bool stickyFloor>
Point Grid<MinX, MaxX, MinY, MaxY, stickyFloor>::sand() {
    for (int x = 500, y = 0;; ) {
        if (y + 1 == floor) {
            if constexpr (stickyFloor)
                (*this)[{x, y}] = SAND;
            return { x, y };
        }
        if ((*this)[{x, y+1}] == AIR)
            y++;
        else if ((*this)[{x-1, y+1}] == AIR)
            x--, y++;
        else if ((*this)[{x+1, y+1}] == AIR)
            x++, y++;
        else {
            (*this)[{x, y}] = SAND;
            return {x, y};
        }
    }
}

Point movetoward(const Point &cur, const Point &dest) {
    auto [ curx, cury ] = cur;
    auto [ destx, desty ] = dest;
    auto diffx = destx - curx;
    auto diffy = desty - cury;
    auto deltax = diffx == 0 ? 0 : diffx < 0 ? -1 : 1;
    auto deltay = diffy == 0 ? 0 : diffy < 0 ? -1 : 1;
    return { curx + deltax, cury + deltay };
}

Point getpos(std::istream &is) {
    int x, y;
    char _;
    is >> x >> _ >> y;
    assert(_ == ',');
    return {x, y};
};

template <int MinX, int MaxX, int MinY, int MaxY, bool stickyFloor>
Grid<MinX, MaxX, MinY, MaxY, stickyFloor>::Grid(std::istream &is) : storage(SizeX * SizeY, AIR) {
    std::string l;
    while (getline(is, l)) {
        std::istringstream is(l);
        Point start = getpos(is);
        for (;;) {
            std::string arrow;
            is >> arrow;
            assert(arrow == "->" || is.eof());
            if (is.eof())
                break;
            for (auto end = getpos(is);  start != end; start = movetoward(start, end))
                (*this)[start] = ROCK;
        }
        (*this)[start] = ROCK;
    }
    floor = maxy + 2;
}

}

void part1(std::istream &is, std::ostream &os) {
    Grid<-250, 750, 0, 600, false> space(is);
    int count;
    for (count = 0; get<1>(space.sand()) != space.floor - 1; ++count)
        ;
    os << "part1: " << count << "\n";
}

void part2(std::istream &is, std::ostream &os) {
    Grid<-250, 750, 0, 600, true> space(is);
    int count;
    for (count = 1; space.sand() != Point{500,0}; ++count)
        ;
    os << "part2: " << count << "\n";
}
