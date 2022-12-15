%{
#include "solve.h"
#include "parser.h"
%}

DIGIT [0-9]
EOL \n
SPACE [ ]
EQ =
NUMBER [+-]?{DIGIT}+

%%
Sensor      { return SENSOR; }
at          { return AT; }
=           { return EQ; }
,           { return COMMA; }
x           { return X; }
y           { return Y; }
closest     { return CLOSEST; }
:           { return COLON; }
beacon      { return BEACON; }
is          { return IS; }
{EOL}       { return EOL; }
{SPACE}     {}
{NUMBER}    { yylval.number = strtoul(yytext, 0, 0); return NUM; }
