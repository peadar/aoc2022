%{
#include "solve.h"
%}
%parse-param { Network &net }
%token VALVE ID FLOWRATE NUM TUNNELS_LEAD_TO_VALVES COMMA EOL SEMICOLON
%union { int number; char *name; Valve::Set *ids; }
%type<number> NUM;
%type<name> ID;
%type<ids> idlist;
%%
valves: %empty { }
      | valves valve { }
valve: VALVE ID FLOWRATE NUM SEMICOLON TUNNELS_LEAD_TO_VALVES idlist EOL {
     auto vid = net.valveid($2);
     net.valves[vid].name = $2;
     net.valves[vid].flowRate = $4;
     net.valves[vid].connected = std::move(*$7);
}
idlist: idlist COMMA ID { $$ = $1; $$->set(net.valveid($3)); }
  | ID { $$ = new Valve::Set(); $$->set(net.valveid($1)); }
%%
