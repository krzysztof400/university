%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STACK_SIZE 100

int stack[STACK_SIZE];
int top = -1;
int has_error = 0;

void error(const char *msg);

int power(int base, int exp) {
    int res = 1;
    if (exp < 0) return 0;
    for (int i = 0; i < exp; i++) {
        res *= base;
    }
    return res;
}

void push(int val) {
    if (has_error) return;

    if (top >= STACK_SIZE - 1) {
        error("Error: Stack overflow");
    } else {
        stack[++top] = val;
    }
}

int pop() {
    if (has_error) return 0;

    if (top < 0) {
        error("Error: Too few arguments");
        return 0;
    }
    return stack[top--];
}

void bin_op(char op) {
    if (has_error) return;
    
    int b = pop();
    int a = pop();

    if (has_error) return;

    switch(op) {
        case '+': push(a + b); break;
        case '-': push(a - b); break;
        case '*': push(a * b); break;
        case '^': push(power(a, b)); break;
        case '/':
            if (b == 0) error("Error: Division by zero");
            else push(a / b);
            break;
        case '%':
            if (b == 0) error("Error: Modulo by zero");
            else push(a % b);
            break;
    }
}

%}

%%

[ \t]+          { }

-?[0-9]+      { push(atoi(yytext)); }

"+"             { bin_op('+'); }
"-"             { bin_op('-'); }
"*"             { bin_op('*'); }
"/"             { bin_op('/'); }
"%"             { bin_op('%'); }
"^"             { bin_op('^'); }

\n              {
                //   if (!has_error) {
                //       if (top == 0) {
                //           printf("= %d\n", stack[top]);
                //       } else if (top > 0) {
                //           error("Error: Too few operators");
                //       }
                //   }
                  
                  has_error = 0;
                  top = -1;
                }

.               { 
                  char msg[100];
                  sprintf(msg, "Error: Bad symbol \"%s\"", yytext);
                  error(msg);
                }

%%

void error(const char *msg) {
    if (has_error) return;
    
    printf("%s\n", msg);
    has_error = 1;
    
    char c;
    while ((c = input()) != '\n' && c != EOF) { }

    top = -1;
}

int yywrap() {
    return 1;
}

int main(int argc, char *argv[]) {
    printf("RPN Calculator ready. Enter expressions.\n");
    yylex();
    return 0;
}