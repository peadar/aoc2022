%{
#include "solve.h"
#include "parser.h"
%}
%%
Blueprint   { return BLUEPRINT; }
robot       { return ROBOT; }
costs       { return COSTS; }
and         { return AND; }
Each        { return EACH; }
\n          { }
:           { return COLON; }
\.           { return STOP; }
[ ]         {}
[0-9]+      { yylval.number = strtoul(yytext, 0, 0); return NUM; }
ore         { yylval.material = ORE; return MATERIAL; }
clay        { yylval.material = CLAY; return MATERIAL; }
obsidian    { yylval.material = OBSIDIAN; return MATERIAL; }
geode       { yylval.material = GEODE; return MATERIAL; }
