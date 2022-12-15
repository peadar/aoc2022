#include <vector>

extern "C" {
int yylex();
int yyparse();
int yyerror(const char *);
int yywrap();
}

struct Point {
    int x, y;
};

struct Sensor {
    Point location;
    Point closestBeacon;
};


extern std::vector<Sensor> sensors;
