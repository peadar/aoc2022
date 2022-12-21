#include "solve.h"
#include <unistd.h>
#include <algorithm>
#include <cassert>
#include <iostream>
#include <tuple>

// Find the distance between two nodes.
int Network::shortestPath(int start, int end) {
    if (start == end)
        return 0;

    // We use "distances" to store previously calculated distances. The graph
    // is completely symmetric, so we ignore ordering. The key to the cache is
    // NODECOUNT * smallest node id + largest node id
    if (start < end)
        std::swap(start, end);
    auto &dist = distances[end * valves.size() + start];
    if (dist != 0)
        return dist;

    // Not in cache, search breadth-first. All neighbors are distance 1, so we
    // get the shortest path first time we hit the destination.
    Valve::Set visited;
    std::deque<std::pair<int, int>> q;
    q.push_back({ start, 0 });
    while (q.size()) {
        auto [ location, distance ]  = std::move(q.front());
        q.pop_front();
        for (auto dest = 0; dest < valves.size(); ++dest) {
            if (!this->valves[location].connected[dest])
                continue;
            if (dest == end) {
                dist = distance + 1; // insert into cache.
                return dist;
            }
            if (!visited[dest]) {
                q.push_back({dest, distance + 1});
                visited[dest] = 1;
            }
        }
    }
    abort(); // we expect the network to be fully connected.
}

// Find *all* paths through postiive-flow-rate valves from "AA", with a max
// travel time of "time". Sorted by cumulative flow rate.
std::vector<PathInfo> Network::findAllPaths(int time) {
    auto nonz = positiveFlowValves();
    PathInfo pi {};
    pi.timeLeft = time;
    pi.valve = valveid("AA");
    std::vector<PathInfo> paths{ std::move(pi) };
    paths.reserve(60000);
    // Create new paths one step longer than what's there, until they are all
    // either compplete (unlikely), or exceed the time budget.
    for (int i = 0; i < paths.size(); ++i) {
        auto pi = paths[i]; // need to copy, as the push-back below may invalidate.
        for (int valve = 0; valve < valves.size(); ++valve) {
            if (pi.visited[valve] || !nonz[valve])
                // we've been here before, or zero-flow node, so don't care
                continue;
            auto distance = shortestPath(pi.valve, valve);
            if (distance + 1 > pi.timeLeft)
                continue; // we'd exceed the time limit visiting here - ignore.
            // Create a new path - copy the old one, and update.  we could
            // store the index of the previous PathInfo so we could work out
            // the actual path themselves later, but it's not strictly
            // necessary so we don't bother - just keep track of the cumulative
            // flow for this path and the remaining time.
            paths.push_back(pi);
            auto &neu = paths.back();
            neu.valve = valve;
            neu.timeLeft -= distance + 1;
            neu.cumulativeFlow += valves[valve].flowRate * neu.timeLeft;
            neu.visited[valve] = true;
        }
    }
    // sort by reversed cumulativeFlow - best paths at the front of the vector.
    std::sort(paths.begin(), paths.end(), [](const PathInfo &l, const PathInfo &r)
          { return l.cumulativeFlow > r.cumulativeFlow; } );
    return paths;
}

Valve::Set Network::positiveFlowValves() {
    Valve::Set start;
    for (int i = 0; i < valves.size(); ++i)
        if (valves[i].flowRate > 0)
            start[i] = true;
    return start;
}

namespace {

void part1(Network &net) {
    // we don't actually need all paths, but we will for part2, so just find all, and pick the best.
    auto all = net.findAllPaths(30);
    std::cout << "part1: " << all.begin()->cumulativeFlow << "\n";
}

void part2(Network &net) {
    auto all = net.findAllPaths(26);
    // Pick the top paths that are disjoint. Look at candidate for the better
    // path, then look at all worse paths until we know we can't improve on
    // what we already have
    int best = 0;
    int maxj = all.size();
    for (int i = 0; i < maxj - 1; ++i) {
        if (all[i].cumulativeFlow < best/2)
            break; // if the better flow rate is < half the best, then we can't improve
        for (int j = i+1; j < maxj; ++j) {
            Valve::Set isect = all[i].visited & all[j].visited;
            if (isect.none() && best < all[i].cumulativeFlow + all[j].cumulativeFlow) {
                best = all[i].cumulativeFlow + all[j].cumulativeFlow;
                maxj = j; // our second-best path must be better than this, and disjoint from a worse best.
            }
        }
    }
    std::cout << "part2: " << best;
}

}

int main(int argc, char *argv[]) {
    bool do_draw = false;
    extern FILE *yyin;
    for (int c; (c = getopt(argc, argv, "gf:")) != -1; ) {
        switch (c) {
            case 'f':
                yyin = fopen(optarg, "r");
                assert(yyin);
                break;
            case 'g':
                do_draw = true;
                break;
        }
    }
    Network net;
    yyparse(net);
    if (do_draw) {
        std::cout << "graph { \n";
        for (int i = 0; i < net.valves.size() - 1; ++i) {
            auto &valve = net.valves[i];
            for (int j = i + 1; j < net.valves.size(); ++j) {
                if (valve.connected[j])
                    std::cout << "\t" << valve.name << " -- " << net.valves[j].name << "\n";
            }
        }
        std::cout << "}\n";
        return 0;
    }
    part1(net);
    part2(net);
    return 0;
}
