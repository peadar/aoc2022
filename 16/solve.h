#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <set>

struct Valve {
using Connected = std::set<Valve *>;
    std::string name;
    int flowRate;
    Connected connected;
};

using Network = std::map<std::string, Valve>;

extern "C" {
int yylex();
int yyparse(Network &);
inline int yywrap() { return true; };
inline int yyerror(Network &, const char *p) { throw std::runtime_error(p); }
}
