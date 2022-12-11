#include <unordered_map>
#include <limits>
#include <iostream>
#include <memory>
#include <string>
#include <variant>
#include <assert.h>
#include "aoc.h"

class Dir;
struct Invalid{};
using DirNode = std::unique_ptr<Dir>;
using Size = unsigned long;
using DirEnt = std::variant<Invalid, Size, DirNode>;

constexpr bool isdir(const DirEnt &ent) { return std::holds_alternative<DirNode>(ent); }

Dir *asdir(DirEnt &ent) {
    assert(isdir(ent));
    return std::get<DirNode>(ent).get();
}

class Dir {
    Size cachedSize;
public:
    Dir *parent;
    void addent(const std::string &line);
    std::unordered_map<std::string, DirEnt> entries;
    Dir(Dir *parent = nullptr) : parent(parent) , cachedSize(std::numeric_limits<Size>::max()) {}
    Size size();
};

Size Dir::size() {
    if (cachedSize == std::numeric_limits<Size>::max()) {
        cachedSize = 0;
        for (auto & [name, ent ] : entries) {
            struct {
                Size operator()(Size &sz) const { return sz; }
                Size operator()(DirNode &d) const { return d->size(); }
                Size operator()(Invalid &) const { abort(); return 0; }
            } cb;
            cachedSize += std::visit(cb, ent);
        }
    }
    return cachedSize;
}

void Dir::addent(const std::string &to) {
    // split into type/size and name
    auto [ type, component ] = aoc::token(to);
    auto &entry = entries[component];
    if (std::holds_alternative<Invalid>(entry)) {
        if (type == "dir")
            entry = std::make_unique<Dir>(this);
        else
            entry = std::stoul(type);
    }
}

Size part1(Dir &d) {
    Size tot = 0;
    if (d.size() < 100000)
        tot += d.size();
    for (auto &[ name, ent ] : d.entries)
        if (isdir(ent))
            tot += part1(*asdir(ent));
    return tot;
}

Size part2(Dir &d, Size min) {
    auto bestof = [min](Size val, Size best) { return val >= min ? std::min(best, val) : best; };
    Size best = std::numeric_limits<Size>::max();
    for (auto &[ name, ent ] : d.entries)
        if (isdir(ent))
            best = bestof( part2(*asdir(ent), min), best );
    return bestof(d.size(), best);
}

int main(int argc, char *argv[]) {
    Dir root;
    Dir *cd = &root;
    while (std::cin.good()) {
        std::string line;
        std::getline(std::cin, line);
reprocess:
        if (!std::cin.good())
            break;
        if (line.rfind("$ cd ", 0) == 0) {
            line = line.substr(5);
            cd = line == "/" ? &root : line == ".." ? cd->parent : asdir(cd->entries[line]);
        } else if (line.rfind("$ ls", 0) == 0) {
            while (std::cin.good()) {
                std::getline(std::cin, line);
                if (!std::cin.good() || line[0] == '$')
                    goto reprocess;
                cd->addent(line);
            }
        } else {
            throw std::runtime_error((std::string("unexpected input ") + line).c_str());
        }
    }
    std::cout << "part1: " << part1(root) << std::endl;
    constexpr Size capacity = 70000000, targetUnused = 30000000, targetUsed = capacity - targetUnused;
    std::cout << "part2: " << part2(root, root.size() - targetUsed) << "\n";
}
