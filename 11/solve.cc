#include "aoc.h"
#include <deque>
#include <iostream>
#include <numeric>
#include <optional>
using namespace std;

namespace {

class Monkey {
    string name;
    deque<long> items;
    char op;
    optional<long> value;
    int truedest, falsedest;
public:
    int inspectionCount = 0;
    bool busy() { return !items.empty(); }
    long modulus;
    inline pair<long /*new value*/, int /*destination*/ > inspect(long divisor, long modulus);
    void take(long item) { items.push_back(item); }
    inline long worry(long);
    Monkey(istream &);
};

vector<Monkey> parseInput(istream &is);

long Monkey::worry(long in) {
    long rhs = value ? *value : in;
    switch (op) {
        case '+': return in + rhs;
        case '*': return in * rhs;
        default:
                  throw runtime_error("unsupported operation");
    }
}

pair<long, int> Monkey::inspect(long divisor, long lcm) {
    long item = worry(items.front()) / divisor % lcm;
    items.pop_front();
    inspectionCount++;
    return { item, item % modulus == 0 ? truedest : falsedest };
}

long solve(istream &is, int divisor, int iterations) {
    auto monkeys = parseInput(is);
    long modulus = std::accumulate( monkeys.begin(), monkeys.end(), 1,
            [](long acc, const Monkey &in) { return acc * in.modulus; });
    for (int i = 0; i < iterations; ++i)
        for (auto &m : monkeys)
            while (m.busy()) {
                auto [ value, destination ] = m.inspect(divisor, modulus);
                monkeys[destination].take(value);
            }
    long best1 = 0, best2 = 0; // This is more efficient than sorting and picking the top 2 - O(num monkeys)
    for (auto &m : monkeys)
        if (m.inspectionCount > best1) {
            best1 = m.inspectionCount;
            if (best1 > best2)
                swap(best1, best2);
        }
    return best1 * best2;
}

pair<string, string> splitline(std::istream &is, const string &sep) {
    string l;
    getline(is, l);
    return aoc::token(l, sep);
}

template <typename T> void asvec(T& container,  std::string l, const string_view sep, typename T::value_type xform(string_view) ) {
    while (l != "") {
        auto [ tok, rest ] = aoc::token(l, sep);
        l = rest;
        container.push_back(xform(tok));
    }
}

Monkey::Monkey(istream &is) {
    name = get<0>(splitline(is, ":"));
    auto starting = get<1>(splitline(is, "Starting items: "));
    asvec(items, starting, ", ", [](string_view sv) { return stol(string(sv)); });
    auto operation = get<1>(splitline(is, ": new = old "));
    std::vector<string> ops;
    asvec(ops, operation, " ", [](string_view sv) { return string(sv); });
    op = ops[0][0];
    value = ops[1] == "old" ? optional<long>() : optional(stol(ops[1]));
    modulus = stol(get<1>(splitline(is, "Test: divisible by ")));
    truedest = stoi(get<1>(splitline(is, "If true: throw to monkey ")));
    falsedest = stoi(get<1>(splitline(is, "If false: throw to monkey ")));
    splitline(is, ""); // blank
}

vector<Monkey> parseInput(istream &is) {
    vector<Monkey> monkeys;
    while (is.good())
        monkeys.emplace_back(is);
    return monkeys;
}
}

void part1(istream &in, ostream &out) { out << "part1: " << solve(in, 3, 20) << endl; }
void part2(istream &in, ostream &out) { out << "part2: " << solve(in, 1, 10000) << endl; }
