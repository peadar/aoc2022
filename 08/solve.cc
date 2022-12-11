#include <vector>
#include <unistd.h>
#include <string>
#include <functional>
#include <iostream>
using Forest = std::vector<std::vector<int>>;
using Visibles = std::vector<std::vector<bool>>;

template <typename T>
int
doLine(const Forest &g, Visibles &vis, int row, int col, T update) {
    int highest = -1;
    int newVis = 0;
    do {
        int h = g[row][col];
        if (h > highest)  {
            if (!vis[row][col])
                newVis++;
            vis[row][col] = true;
            highest = h;
        }
    } while (update(row, col));
    return newVis;
}

int doTree(const Forest &g, int row, int col) {
    if (row == g.size() - 1 || col == g[0].size() - 1 || row == 0 || col == 0)
        return 0;
    int h = g[row][col];
    struct Rc { int row, col, dr, dc; };
    Rc dirs[] {
        { row + 1, col    ,  1,  0 } ,
        { row - 1, col    , -1,  0 } ,
        { row,     col + 1,  0,  1 },
        { row,     col - 1,  0, -1 } ,
    };
    int total = 1;
    for (auto &rc : dirs) {
        while (rc.row < g.size() - 1
                && rc.col < g[rc.row].size() - 1
                && rc.row != 0 && rc.col != 0
                && g[rc.row][rc.col] < h
                ) {
            rc.row += rc.dr;
            rc.col += rc.dc;
        }
        total *= abs(rc.row - row) + abs(rc.col - col);
    }
    return total;
}

int
main() {
    Visibles vis;
    Forest input;
    for (;;) {
        std::string s;
        std::getline(std::cin, s);
        if (!std::cin.good() || s == "")
            break;
        input.emplace_back();
        vis.emplace_back();
        auto &row = input.back();
        for (auto c : s)
            row.push_back(c - '0');
        vis.back().resize(row.size());
    }
    int totalVisible = 0;
    for (int row = 0; row < input.size(); ++row) {
        totalVisible += doLine(input, vis, row, 0, [&input] (int &r, int &c) { return ++c != input[r].size(); });
        totalVisible += doLine(input, vis, row, input[row].size() - 1, [&input] (int &r, int &c) { return --c >= 0; });
    }
    for (int col = 0; col < input[0].size(); ++col) {
        totalVisible += doLine(input, vis, 0, col, [&input] (int &r, int &c) { return ++r != input.size(); });
        totalVisible += doLine(input, vis, input.size() - 1, col, [&input] (int &r, int &c) { return --r >= 0; });
    }
    std::cout << "part1: " << totalVisible << std::endl;

    int bestrow, bestcol, besth = 0;
    for (int row = 0; row < input.size(); ++row) {
        for (int col = 0; col < input[row].size(); ++col) {
            int h = doTree(input, row, col);
            if (h > besth) {
                besth = h;
                bestrow = row;
                bestcol = col;
            }
        }
    }
    std::cout << "part2: " << besth << std::endl;
}
