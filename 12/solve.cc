#include "aoc.h"
#include <iterator>
#include <cassert>
#include <queue>
#include <limits>
#include <unistd.h>

using namespace std;

namespace {

struct Node {
    static constexpr int DISTMAX = numeric_limits<int>::max();
    int distance = DISTMAX;
    bool visited = false;
    bool queued = false;
    char height;
    Node(char height) : height(height) {}
};

struct Grid {
    // Manage x,y locations manually into a flat "storage" vectof for our
    // nodes, so we can use a pointer-to-node to work out it's x,y location
    // without storing it elsewhere
    vector<Node> storage;
    int columns = 0, rows = 0, start;
    tuple<int, int>locate(const Node *p) { // node-pointer to x,y location
        auto off = p - &storage[0];
        return { off % columns, off / columns };
    }
    Node *at(int x, int y) { // x,y location to node pointer.
        return &storage[y * columns + x];
    }
    void dijkstra();
    int bestA = Node::DISTMAX;
    int bestS = Node::DISTMAX;
    Grid(istream &in);
};

char mapheight(char in) { return in == 'S' ? 'a' : in == 'E' ? 'z' : in; }

Grid::Grid(istream &is) {
    is >> noskipws;
    int curcols = 0;
    for (auto cp = istream_iterator<char>(is); cp != istream_iterator<char>(); ++cp) {
        switch (*cp) {
            case '\n':
                assert(columns == curcols || columns == 0);
                columns = curcols;
                curcols = 0;
                continue;
            case 'E': // we start at the end, because we're interested in multiple origins
                start = storage.size();
                break;
        }
        curcols++;
        storage.emplace_back(*cp);
    }
    rows = storage.size() / columns;
    dijkstra();
}

void
Grid::dijkstra() {
    // We should use a priority queue here, but not much point for this scale,
    // and the std::priority_queue is insufficient.
    deque<Node *> remaining;
    // Set up our starting node as the front of Q.
    storage[start].distance = 0;
    remaining.push_back(&storage[start]);
    while (remaining.size() && (bestS == Node::DISTMAX || bestA == Node::DISTMAX) ) {
        size_t best = 0;
        for (size_t i = 0; i < remaining.size(); ++i)
            if (remaining[best]->distance > remaining[i]->distance)
                best = i;
        Node *n = remaining[best];
        swap(remaining[best], remaining[remaining.size() - 1]);
        remaining.pop_back();
        // This node has its shortest path set, and it is the shortest path
        // we'll see for the node types we care about - done if both answers
        // are available.
        if (n->height == 'S' && bestS == Node::DISTMAX)
            bestS = n->distance;
        if (mapheight(n->height) == 'a' && bestA == Node::DISTMAX)
            bestA = n->distance;
        // Visit the neighbors adjacencies.
        auto [x, y] = locate(n);
        auto updateDistance = [&] (int x, int y) {
            if (x < 0 || x > columns || y < 0 || y > rows)
                return;
            Node *to = at(x, y);
            if (mapheight(n->height) - mapheight(to->height) > 1)
                return;
            if ( to->distance <= n->distance + 1)
                return;
            to->distance = n->distance + 1;
            if (!to->queued) {
                remaining.push_back(to);
                // we don't need to clear this - when items are removed, they
                // stay removed.
                to->queued = true;
            }
        };
        updateDistance(x + 1, y);
        updateDistance(x - 1, y);
        updateDistance(x, y + 1);
        updateDistance(x, y - 1);
    }
}

}

void part1(istream &in, ostream &out) {
    Grid grid{in};
    out << "part1: " << grid.bestS << "\n";
}

void part2(istream &in, ostream &out) {
    Grid grid{in};
    out << "part2: " << grid.bestA << "\n";
}
