%{
#include <stdio.h>
#include <string.h>

int keep_doxygen = 0;
%}

%x STRING
%x CHAR
%x REM_BLOCK
%x REM_LINE
%x KEEP_BLOCK
%x KEEP_LINE

%%

\"          { ECHO; BEGIN(STRING); }
\'          { ECHO; BEGIN(CHAR); }

<STRING>\\. { ECHO; }
<STRING>\"  { ECHO; BEGIN(INITIAL); }
<STRING>.   { ECHO; }

<CHAR>\\.   { ECHO; }
<CHAR>\'    { ECHO; BEGIN(INITIAL); }
<CHAR>.     { ECHO; }

"/**"|"/*!" {
    if (keep_doxygen) {
        ECHO; BEGIN(KEEP_BLOCK);
    } else {
        BEGIN(REM_BLOCK);
    }
}

"///"|"//!" {
    if (keep_doxygen) {
        ECHO; BEGIN(KEEP_LINE);
    } else {
        BEGIN(REM_LINE);
    }
}

"/*"    { BEGIN(REM_BLOCK); }
"//"    { BEGIN(REM_LINE); }

<REM_BLOCK>"*/"    { BEGIN(INITIAL); }
<REM_BLOCK>.|\n    { ; }

<REM_LINE>\n       { ECHO; BEGIN(INITIAL); }
<REM_LINE>.        { ; }
<REM_LINE>\\(\n|.) { ; }

<KEEP_BLOCK>"*/"   { ECHO; BEGIN(INITIAL); }
<KEEP_BLOCK>.|\n   { ECHO; }

<KEEP_LINE>\n      { ECHO; BEGIN(INITIAL); }
<KEEP_LINE>.       { ECHO; }


.|\n    { ECHO; }

%%

int main(int argc, char **argv) {
    if (argc > 1) {
        if (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--doc") == 0) {
            keep_doxygen = 1;
        }
    }

    yylex();
    return 0;
}

int yywrap() { return 1; }