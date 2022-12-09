#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cassert>

class Coord {
    int x, y;
public:
    constexpr bool operator < (const Coord &rhs) const { return x < rhs.x || (x == rhs.x && y < rhs.y); }
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

static Coord direction(char d) {
    switch (d) {
        case 'U': return {  0, -1 };
        case 'D': return {  0,  1 };
        case 'L': return { -1,  0 };
        case 'R': return {  1,  0 };
        default: throw std::runtime_error("invalid direction");
    }
}

int solve(std::fstream input, int length) {
    assert(length > 0);
    std::vector<Coord> snake(length);
    std::set<Coord> visited { snake.back() };
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

int main(int, char *argv[]) {
    int sizes[] { 2, 10 };
    for (int i = 0; i < 2; ++i)
        std::cout << "part " << i + 1 << " " << solve(std::fstream(argv[1]), sizes[i]) << "\n";
}
