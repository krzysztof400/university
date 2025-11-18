%{
    #include <stdio.h>
    int lines = 0;
    int words = 0;
%}

WORD    [^ \t\n]+
%%
^[ \t]*\n {} // Empty lines

^[ \t]+ { } // White space at the beginning of a line

[ \t]+\n { printf("\n"); lines++; } // White space at the end of a line

[ \t]+      { printf(" "); } // White space between words

\n      { lines++; printf("\n"); } // New line

{WORD}  { words++; ECHO; } // Words
%%

int main() {
    yylex();
    printf("\n\nStatistics:\n");
    printf("Total lines: %d\n", lines);
    printf("Total words: %d\n", words);
    return 0;
}

int yywrap() {
    return 1;
}