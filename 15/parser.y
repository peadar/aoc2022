%{
#include "solve.h"
#include <stdexcept>
%}

%token SENSOR AT X EQ NUM COMMA Y COLON CLOSEST BEACON IS EOL
%union { int number; Point point; Sensor sensor; }
%type<number> NUM;
%type<point> point;
%type<sensor> sensor;
%type<point> closestBeacon;
%type<point> location;
%type<void> sensors;
%%

sensors: %empty { }
       | sensors sensor { sensors.push_back($2); } ;

point: AT X EQ NUM COMMA Y EQ NUM { $$ = { $4, $8 }; } ;
location: SENSOR point { $$ = $2; } ;

closestBeacon: CLOSEST BEACON IS point { $$ = $4; } ;

sensor: location COLON closestBeacon EOL {
      $$.location = $1;
      $$.closestBeacon = $3;

} ;

%%

extern "C" {
int yywrap() { return true; };
int yyerror(const char *p) { throw std::runtime_error(p); }
}
