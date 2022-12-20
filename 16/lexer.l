%{
#include "solve.h"
#include "parser.h"
%}
%%
Valve       { return VALVE; }
has\ flow\ rate= { return FLOWRATE; }
\n          { return EOL; }
;           { return SEMICOLON; }
,           { return COMMA; }
tunnels?\ leads?\ to\ valves?  { return TUNNELS_LEAD_TO_VALVES; }
[ ]         {}
[0-9]+      { yylval.number = strtoul(yytext, 0, 0); return NUM; }
[a-zA-Z]+   { yylval.name = strdup(yytext); return ID; }
