#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include "solve.h"
#include <unistd.h>

extern FILE *yyin;
std::vector<Sensor> sensors;

namespace {

using range = std::pair<int, int>;
bool
operator < (const range &l, const range &r) {
    auto [ ls, le ] = l;
    auto [ rs, re ] = r;
    return ls < rs;
}

int manhattan(const Point &lhs, const Point &rhs) { return abs(lhs.x - rhs.x) + abs(lhs.y - rhs.y); }

std::vector<range>
rangesForRow(int row, int min, int max) {
    std::vector<range> ranges;
    for (auto &s : sensors) {
        int extent = manhattan(s.closestBeacon, s.location) - abs(s.location.y - row);
        int start = s.location.x - extent;
        int end = s.location.x + extent + 1;
        if (extent < 0 || end < min || start >= max)
            continue;
        ranges.emplace_back(std::max(min, start), std::min(max, end));
    }
    std::sort(ranges.begin(), ranges.end());
    return ranges;
}

void part1(int row) {
    auto ranges = rangesForRow(row, std::numeric_limits<int>::min(), std::numeric_limits<int>::max() - 1);
    // need to discount the beacons that are in our catchment area.
    std::set<int> beaconsOnRow; // note this is ordered.
    for (auto &s : sensors)
        if (s.closestBeacon.y == row)
            beaconsOnRow.insert(s.closestBeacon.x);
    int s = std::numeric_limits<int>::min(), e = s, tot = 0;
    auto beacons = beaconsOnRow.begin();
    for (auto range : ranges) {
        auto [ news, newe ] = range;
        assert(news >= s);
        if (newe <= e) // subset of existing range - ignore.
            continue;
        if (news > e) { // new range is disjoint - consume the current range
            tot += e - s;
            s = news;
        }
        e = newe;
        while (beacons != beaconsOnRow.end() && *beacons >= s && *beacons < e) {
            tot--;
            ++beacons;
        }
    }
    tot += e - s;
    std::cout << "part1: " << tot << "\n";
}

std::optional<int> scanRowForPossible(int row, int maxval) {
    auto ranges = rangesForRow(row, 0, maxval);
    int s = 0;
    int e = 0;
    for (auto &[news, newe] : ranges) {
        if (news > e)
            return e;
        e = std::max(e, newe);
    }
    return std::optional<int>{};
}

void part2(int max) {
    for (int y = 0; y <= max; ++y) {
        auto x = scanRowForPossible(y, max);
        if (x) {
            std::cout << "part2: " << long(*x) * 4000000L + y << "\n";
            break;
        }
    }
}
}

int main(int argc, char *argv[]) {
    int col = 2000000;
    int maxv = 4000000;
    for (int c; (c = getopt(argc, argv, "f:c:m:")) != -1; ) {
        switch (c) {
            case 'f':
                yyin = fopen(optarg, "r");
                assert(yyin);
                break;
            case 'c':
                col = strtoul(optarg, 0, 0);
                break;
            case 'm':
                maxv = strtoul(optarg, 0, 0);
        }
    }
    yyparse();
    part1(col);
    part2(maxv);
}
