#include <cmath>
#include <functional>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <cassert>
namespace {
struct Coord {
    int x, y;
    constexpr bool operator < (const Coord &rhs) const { return x < rhs.x || (x == rhs.x && y < rhs.y); }
    constexpr bool operator == (const Coord &rhs) const { return x == rhs.x && y == rhs.y; }
    constexpr Coord &operator += (const Coord &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    constexpr bool adjacent(const Coord &rhs) const { return std::abs(x - rhs.x) <= 1 && std::abs(y - rhs.y) <= 1; }
    constexpr void movetoward(const Coord &target) {
        x += target.x == x ? 0 : target.x > x ? 1 : -1;
        y += target.y == y ? 0 : target.y > y ? 1 : -1;
    }
    constexpr Coord(const int x, const int y) : x(x), y(y) {}
    Coord() = default;
};
}

namespace std {
template <> struct hash<Coord> { size_t operator()(const Coord &c) const {
    return hash<int>{}(c.x) ^ std::hash<int>{}(c.y); } };
}
namespace {

static Coord direction(char d) {
    switch (d) {
        case 'U': return {  0, -1 };
        case 'D': return {  0,  1 };
        case 'L': return { -1,  0 };
        case 'R': return {  1,  0 };
        default: throw std::runtime_error("invalid direction");
    }
}

int solve(std::istream &input, int length) {
    assert(length > 0);
    std::vector<Coord> snake(length);
    std::unordered_set<Coord> visited { snake.back() };
    for (std::string s; std::getline(input, s); ) {
        Coord dir = direction(s[0]);
        for (auto i = stoi(s.substr(2)); i; --i) {
            snake[0] += dir;
            for (size_t pos = 1; pos < snake.size() && !snake[pos].adjacent(snake[pos-1]); ++pos)
                snake[pos].movetoward(snake[pos-1]);
            visited.insert(snake.back());
        }
    }
    return visited.size();
}
}

void part1(std::istream &is, std::ostream &os) { os << "part1: " << solve(is, 2) << std::endl; }
void part2(std::istream &is, std::ostream &os) { os << "part2: " << solve(is, 10) << std::endl; }
