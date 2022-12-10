#include <functional>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>

namespace {

struct Machine {
    using Register = long;
    std::function<void(const Machine &)> watch;
    struct Noop {};
    struct Addx { Register value; };
    using Insn = std::variant<Noop, Addx>;
    static Insn parse(std::string line);
    Register x, cycle;
    void operator() (const Noop&) { tick(); }
    void operator() (const Addx& addx) {
        tick(2);
        x += addx.value;
    }
    Machine(std::function<void(const Machine &)> watch) : watch(watch), x(1), cycle{1} {}
    void tick(int count = 1) {
        while (count--) {
            watch(*this);
            ++cycle;
        }
    }
    void run(std::istream &input);
};

Machine::Insn Machine::parse(std::string line) {
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

void Machine::run(std::istream &input) {
    for (std::string s; std::getline(input, s); )
        std::visit(*this, parse(s));
    tick();
}

}

void part1(std::istream &in, std::ostream &out) {
    Machine::Register part1 = 0;
    Machine{ [&part1](const Machine &m) {
        if ((m.cycle - 20) % 40 == 0 && m.cycle <= 220)
            part1 += m.cycle * m.x;
    }}.run(in);
    out << "part1: " << part1 << std::endl;
}

void part2(std::istream &in, std::ostream &out) {
    out << "part2:\n";
    Machine {[&out](const Machine &m) {
        Machine::Register rasterpos = (m.cycle - 1) % 40;
        out << (std::abs(rasterpos - m.x) <= 1 ? "#" : " ");
        if (m.cycle % 40 == 0)
            out << "\n";
    }}.run(in);
}
