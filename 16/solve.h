#include <bitset>
#include <stdexcept>
#include <deque>
#include <map>
#include <string>
#include <vector>

// Really an int that defaults to -1 So we can detect it when auto-inserted
// into a map.
struct ValveId {
    int id;
    ValveId() : id(-1) {}
    ValveId &operator=(int id_) { id = id_; return *this; }
    operator int () const { return id; }
};

struct Valve {
    using Set = std::bitset<64>; // using a bitset is worth it here - intersection is trivial, no allocation overhead, etc.
    std::string name;
    int flowRate;
    Set connected;
};

struct PathInfo {
    int valve; // the valve visited at the end of this path.
    int timeLeft;
    int cumulativeFlow;
    Valve::Set visited;
    PathInfo(const PathInfo &rhs) = default;
    PathInfo() = default;
};

class Network {
    int shortestPath(int start, int end);
    Valve::Set positiveFlowValves();
    std::map<std::string, ValveId> name2idx;
    std::map<int, int> distances;
public:
    std::vector<Valve> valves;
    int valveid(const std::string &name) {
        auto &vid = name2idx[name];
        if (vid == -1) {
            vid = valves.size();
            valves.push_back({});
        }
        return vid;
    }
    std::vector<PathInfo> findAllPaths(int time);
};

extern "C" {
int yylex();
int yyparse(Network &);
inline int yywrap() { return true; };
inline int yyerror(Network &, const char *p) { throw std::runtime_error(p); }
}
