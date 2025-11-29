%{
#include <stdio.h>
#include <stdlib.h>
#include "calc.tab.h"

#define P 1234577

void yyerror(const char *s);
%}

%option noyywrap

%%

[ \t]+      {  }

\\\n        { }

#.* { }

[0-9]+      { 
                long long val = atoll(yytext);
                yylval.ival = val % P;
                return NUM; 
            }

"+"         { return ADD; }
"-"         { return SUB; }
"*"         { return MUL; }
"/"         { return DIV; }
"^"         { return POW; }
"("         { return L_PAREN; }
")"         { return R_PAREN; }
\n          { return EOL; }

.           {
              printf("Unknown symbol: %c\n", *yytext); 
            }

%%