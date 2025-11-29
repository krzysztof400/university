%{
#include <stdio.h>
#include <stdlib.h>

#define P 1234577

int yylex();
void yyerror(const char *s);

int error_flag = 0;

static inline int mod_p(long long a) {
    int r = a % P;
    return r < 0 ? r + P : r;
}

long long extented_euclid(long long a, long long b, long long *x, long long *y) {
    if (a == 0) {
        *x = 0;
        *y = 1;
        return b;
    }
    long long x1, y1;
    long long gcd = extented_euclid(b % a, a, &x1, &y1);
    *x = y1 - (b / a) * x1;
    *y = x1;
    return gcd;
}

int mod_inverse(int a) {
    long long x, y;
    long long g = extented_euclid(a, P, &x, &y);
    if (g != 1) {
        return 0;
    }
    return mod_p(x);
}

int mod_pow(int base, int exp) {
    long long res = 1;
    long long b = base;
    int e = exp;

    // Nonpositive POW: a^(-n) = (a^-1)^n
    if (e < 0) {
        b = mod_inverse(b);
        if (b == 0) return 0;
        e = -e;
    }

    // Square and Multiply
    while (e > 0) {
        if (e % 2 == 1) res = (res * b) % P;
        b = (b * b) % P;
        e /= 2;
    }
    return (int)res;
}

%}

%union {
    int ival;
}

%token <ival> NUM
%token ADD SUB MUL DIV
%token EOL
%token L_PAREN R_PAREN

%left ADD SUB
%left MUL DIV
%nonassoc POW
%precedence UMINUS

%type <ival> expr

%%

input:
    | input line
    ;

line: EOL 
    | expr EOL { 
        if (!error_flag) {
            printf("\nResult: %d\n", $1); 
        } else {
            error_flag = 0;
        }
    }
    | error EOL {
        error_flag = 0;
        yyerrok;
    }
    ;

expr: NUM { 
        int val = mod_p($1);
        printf("%d ", val); 
        $$ = val; 
    }
    | expr ADD expr { 
        if (!error_flag) {
            printf("+ "); 
            $$ = mod_p((long long)$1 + $3); 
        }
    }
    | expr SUB expr { 
        if (!error_flag) {
            printf("- "); 
            $$ = mod_p((long long)$1 - $3); 
        }
    }
    | expr MUL expr { 
        if (!error_flag) {
            printf("* "); 
            $$ = mod_p((long long)$1 * $3); 
        }
    }
    | expr DIV expr { 
        if (!error_flag) {
            printf("/ "); 
            if ($3 == 0) {
                fprintf(stderr, "divisoin by zero.");
                error_flag = 1;
                $$ = 0;
            } else {
                int inv = mod_inverse($3);
                $$ = mod_p((long long)$1 * inv);
            }
        }
    }
    | expr POW expr { 
        if (!error_flag) {
            printf("^ "); 
            if ($1 == 0 && $3 < 0) {
                 yyerror("division by zero.");
                 error_flag = 1;
                 $$ = 0;
            } else {
                 $$ = mod_pow($1, $3); 
            }
        }
    }
    | SUB expr %prec UMINUS { 
        $$ = mod_p(-$2);
        printf("NEG ");
    }
    | L_PAREN expr R_PAREN { 
        $$ = $2; 
    }
    ;

%%

int main() {
    yyparse();
    return 0;
}

void yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    error_flag = 1;
}