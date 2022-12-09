#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>

class Coord {
    int x, y;
public:
    constexpr Coord &operator += (const Coord &rhs) {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    constexpr bool operator == (const Coord &rhs) const { return x == rhs.x && y == rhs.y; }
    constexpr bool operator < (const Coord &rhs) const { return x < rhs.x || (x == rhs.x && y < rhs.y); }
    constexpr bool adjacent(const Coord &rhs) const { return std::abs(x - rhs.x) <= 1 && std::abs(y - rhs.y) <= 1; }
    constexpr void movetoward(const Coord &target) {
        x += target.x == x ? 0 : target.x > x ? 1 : -1;
        y += target.y == y ? 0 : target.y > y ? 1 : -1;
    }
    constexpr Coord(const int x, const int y) : x(x), y(y) {}
    Coord() = default;
};

int
main(int, char *argv[])
{
    static std::map<char, Coord > directions = {
        { 'U', {0, -1} }, { 'D', {0, 1} }, { 'L', {-1, 0} }, { 'R', {1, 0} }
    };
    static constexpr int partSizes[] { 2, 10 };
    for (int part = 0; part < 2; ++part) {
        std::vector<Coord> snake(partSizes[part]);
        std::set<Coord> visited { snake.back() };
        std::string s;
        for (std::ifstream f(argv[1]); std::getline(f, s); ) {
            auto &dir = directions[s[0]];
            for (auto i = stoi(s.substr(2)); i; --i) {
                snake[0] += dir;
                for (size_t pos = 1; pos < snake.size() && !snake[pos].adjacent(snake[pos-1]); ++pos)
                    snake[pos].movetoward(snake[pos-1]);
                visited.insert(snake.back());
            }
        }
        std::cout << "part " << part + 1 << " " << visited.size() << "\n";
    }
}
