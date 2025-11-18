/* SECTION 1: DEFINITIONS */
%{
    /* C Code to be copied verbatim (headers, variables) */
    #include <stdio.h>
%}
/* Regular Expression Aliases (optional macros) */
DIGIT   [0-9]
WORD    [a-zA-Z]+

%%
{DIGIT}+        { printf("Found Integer: %s\n", yytext); }
{WORD}          { printf("Found Word: %s\n", yytext); }
.               { /* Ignore everything else */ }
%%

/* SECTION 3: USER CODE */
/* Helper C functions and main() */
int main() {
    yylex(); /* The function that starts the scanning */
    return 0;
}

int yywrap() {
    return 1; /* Indicates end of input */
}