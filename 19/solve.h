#include <map>
#include <unordered_map>
#include <stdexcept>
#include <vector>

enum Material {
   ORE, CLAY, OBSIDIAN, GEODE, LAST
};
using MaterialMap = std::array<int, Material::LAST>;
using Blueprint = std::unordered_map<Material, MaterialMap>;

struct State {
   std::vector<Blueprint> blueprints;
   MaterialMap costlist;
   Blueprint curblueprint;
};

extern "C" {
int yylex();
int yyparse(State &);
inline int yywrap() { return true; };
inline int yyerror(State &, const char *p) { throw std::runtime_error(p); }
}
