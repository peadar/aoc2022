#include <cmath>
#include <unistd.h>
#include <functional>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cassert>
#include <variant>


using Register = long;

struct Noop {};
struct Addx { Register value; };
using Insn = std::variant<Noop, Addx>;

Insn parse(std::string line) {
    std::vector<std::string> tokens;

    for (size_t space; (space = line.find(' ')) != std::string::npos; ) {
        tokens.push_back(line.substr(0, space));
        line = line.substr(space + 1);
    }
    tokens.push_back(line);
    if (tokens[0] == "noop")
        return Noop{};
    if (tokens[0] == "addx")
        return Addx { std::stoi(tokens[1]) };
    throw std::runtime_error("invalid insn");
}

struct Machine {
    Register x;
    Register cycle;
    Machine(std::function<void(const Machine &)> watch) : x(1), cycle{1}, watch(watch) {}
    std::function<void(const Machine &)> watch;
    void tick(int count = 1) {
        while (count--) {
            watch(*this);
            ++cycle;
        }
    }
    Register signalStrength() const {
        return cycle * x;
    }
    void operator() (const Noop&) {
        tick();
    }
    void operator() (const Addx& addx) {
        tick(2);
        x += addx.value;
    }
    void run(std::fstream &input);
};

void Machine::run(std::fstream &input) {
    input.seekg(0);
    for (std::string s; std::getline(input, s); ) {
        std::visit(*this, parse(s));
    }
    tick();
}

int main(int, char *argv[]) {
    std::fstream in(argv[1]);
    Register part1 = 0;
    Machine m{ [&part1](const Machine &m) {
        if ( (m.cycle - 20) % 40 == 0) {
            assert(m.cycle <= 220);
            part1 += m.signalStrength();
        }
    }};
    m.run(in);
    std::cout << "part1: " << part1 << std::endl;
}
