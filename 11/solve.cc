#include "aoc.h"
#include <deque>
#include <iostream>
#include <numeric>
#include <optional>
using namespace std;

namespace {

struct Monkey {
    string name;
    long inspectionCount = 0;
    deque<long> items;
    char op;
    optional<long> value;
    Monkey(istream &);
    inline long worry(long);
    int truedest;
    int falsedest;
    long modulus;
    inline pair<long, int> inspect(long divisor, long modulus);
    void take(long item) { items.push_back(item); }
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

pair<long, int> Monkey::inspect(long divisor, long bigmod) {
    inspectionCount++;
    long item = items.front();
    items.pop_front();
    item = worry(item) / divisor % bigmod;
    return { item, item % modulus == 0 ? truedest : falsedest };
}

long solve(istream &is, int divisor, int iterations) {
    auto monkeys = parseInput(is);
    long modulus = std::accumulate( monkeys.begin(), monkeys.end(), 1,
            [](long acc, const Monkey &in) { return acc * in.modulus; });
    for (int i = 0; i < iterations; ++i) {
        for (auto &m : monkeys) {
            while (!m.items.empty()) {
                auto [ value, destination ] = m.inspect(divisor, modulus);
                monkeys[destination].take(value);
            }
        }
    }
    // This is more efficient than sorting and picking the top 2.
    long best1 = 0, best2 = 0;
    for (auto &m : monkeys) {
        if (m.inspectionCount > best1) {
            best1 = m.inspectionCount;
            if (best1 > best2)
                swap(best1, best2);
        }
    }
    return best1 * best2;
}
}
void part1(istream &in, ostream &out) { out << "part1: " << solve(in, 3, 20) << endl; }
void part2(istream &in, ostream &out) { out << "part2: " << solve(in, 1, 10000) << endl; }

namespace {
// Parsing stuff
pair<string, string> splitline(std::istream &is, const string_view &sep) {
    string l;
    getline(is, l);
    auto mark = l.find(sep);
    if (mark == string::npos)
        throw runtime_error("unexpected input");
    return { string(l.substr(0, mark)), string(l.substr(mark + sep.size())) };
}

template <typename T> T asvec(string_view l, const string_view sep, typename T::value_type xform(string_view)) {
    T rv;
    for (size_t mark; (mark = l.find(sep)) != string::npos; ) {
        rv.push_back(xform(l.substr(0, mark)));
        l = l.substr(mark + sep.size());
    }
    rv.push_back(xform(l));
    return rv;
}

Monkey::Monkey(istream &is) {
    name = get<0>(splitline(is, ":"));

    auto starting = get<1>(splitline(is, "Starting items: "));
    items = asvec<deque<long>>(starting, ", ", [](string_view sv) { return stol(string(sv)); });

    auto operation = get<1>(splitline(is, ": new = old "));
    auto ops = asvec<vector<string>>(operation, " ", [](string_view sv) { return string(sv); });
    op = ops[0][0];
    value = ops[1] == "old" ? optional<long>() : optional(stol(ops[1]));

    modulus = stol(get<1>(splitline(is, "Test: divisible by ")));
    truedest = stoi(get<1>(splitline(is, "    If true: throw to monkey ")));
    falsedest = stoi(get<1>(splitline(is, "    If false: throw to monkey ")));

    splitline(is, ""); // blank
}

vector<Monkey> parseInput(istream &is) {
    vector<Monkey> monkeys;
    while (is.good())
        monkeys.emplace_back(is);
    return monkeys;
}
}
