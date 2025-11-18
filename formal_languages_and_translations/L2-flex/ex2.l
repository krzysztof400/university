%{
#include <stdio.h>
%}

%x SINGLE_Q
%x DOUBLE_Q
%x TRI_SINGLE
%x TRI_DOUBLE

%%

\\.         { ECHO; }
\"          { ECHO; BEGIN(DOUBLE_Q); }
\'          { ECHO; BEGIN(SINGLE_Q); }
\"\"\"      { ECHO; BEGIN(TRI_DOUBLE); }
\'\'\'      { ECHO; BEGIN(TRI_SINGLE); }

<DOUBLE_Q>\\.   { ECHO; }
<DOUBLE_Q>\"    { ECHO; BEGIN(INITIAL); }
<DOUBLE_Q>.     { ECHO; }

<SINGLE_Q>\\.   { ECHO; }
<SINGLE_Q>\'    { ECHO; BEGIN(INITIAL); }
<SINGLE_Q>.     { ECHO; }

<TRI_DOUBLE>\\.  { ECHO; }
<TRI_DOUBLE>\"\"\"  { ECHO; BEGIN(INITIAL); }
<TRI_DOUBLE>.|\n    { ECHO; }

<TRI_DOUBLE>\\.  { ECHO; }
<TRI_SINGLE>\'\'\'  { ECHO; BEGIN(INITIAL); }
<TRI_SINGLE>.|\n    { ECHO; }

#.* { }

.|\n            { ECHO; }

%%

int main() {
    yylex();
    return 0;
}

int yywrap() { return 1; }