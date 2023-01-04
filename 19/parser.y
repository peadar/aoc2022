%{
#include "solve.h"
#include <iostream>
%}
%parse-param { State &state }
%token BLUEPRINT COLON NUM EACH ROBOT COSTS MATERIAL STOP AND
%union { int number; Material material; }
%type<number> NUM;
%type<material> MATERIAL;
%%
blueprints: %empty { }
      | blueprints blueprint {
      }

robotcosts: %empty {}
     | robotcosts robotcost {}

robotcost: EACH MATERIAL ROBOT COSTS costlist {
         state.curblueprint[$2] = state.costlist;
         state.costlist = {};
         }

costlist: cost AND costlist { }
        | cost STOP {}

cost: NUM MATERIAL {
    state.costlist[$2] = $1;
}

blueprint: BLUEPRINT NUM COLON robotcosts {
         state.blueprints.push_back(state.curblueprint);
         state.curblueprint = {};
}
%%
