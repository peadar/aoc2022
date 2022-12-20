%{
#include "solve.h"
%}
%parse-param { Network &valves }
%token VALVE ID FLOWRATE NUM TUNNELS_LEAD_TO_VALVES COMMA EOL SEMICOLON
%union { int number; char *name; Valve::Connected *ids; }
%type<number> NUM;
%type<name> ID;
%type<ids> idlist;
%%
valves: %empty { }
      | valves valve { }
valve: VALVE ID FLOWRATE NUM SEMICOLON TUNNELS_LEAD_TO_VALVES idlist EOL {
     valves[$2].name = $2;
     valves[$2].flowRate = $4;
     valves[$2].connected = std::move(*$7);
}
idlist: idlist COMMA ID { $$ = $1; $$->insert(&valves[$3]); }
  | ID { $$ = new Valve::Connected(); $$->insert(&valves[$1]); }
%%
