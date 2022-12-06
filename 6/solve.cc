#include <iostream>
#include <map>
#include <fstream>
#include <deque>
#include <string.h>

using namespace std;

static int solve(ifstream &f, int size) {
    string s;
    f.seekg(0);
    getline(f, s);
    if (!f.good()) {
        auto text = std::string("unexpected error reading file. Last error is '") +
            strerror(errno) + "'";
        throw std::runtime_error(text);
    }
    map<char, int> counts;
    deque<char> pipe;
    int uniqCount = 0, loc = 1;
    for (char c : s) {
        pipe.push_back(c);
        int &v = counts[c];
        if (v++ == 0)
            uniqCount++;
        if (pipe.size() > size) {
            char c = pipe.front();
            pipe.pop_front();
            int &v = counts[c];
            if (--v == 0)
                uniqCount--;
        }
        if (uniqCount == size) {
            return loc;
        }
        ++loc;
    }
    throw std::runtime_error("no signal detected");
}

int main(int argc, char *argv[]) {
    ifstream file(argv[1]);
    try {
        std::cout << "part1: " << solve(file, 4) << "\n";
        std::cout << "part2: " << solve(file, 14) << "\n";
        return 0;
    }
    catch (const exception &ex) {
        std::cerr << "error: " << ex.what() << "\n";
    }
}
