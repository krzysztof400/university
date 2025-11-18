%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define STACK_SIZE 100

int stack[STACK_SIZE];
int top = -1;
int has_error = 0; // Flag to stop processing on a line after an error

// Error handling function
void error(const char *msg);

// --- Stack Functions ---
void push(int val) {
    if (has_error) return; // Don't operate if line is already bad

    if (top >= STACK_SIZE - 1) {
        error("Error: Stack overflow");
    } else {
        stack[++top] = val;
    }
}

int pop() {
    if (has_error) return 0;

    if (top < 0) {
        // This error is for operators
        error("Error: Too few arguments");
        return 0; // Return a dummy value
    }
    return stack[top--];
}

// --- Operator Function ---
void bin_op(char op) {
    if (has_error) return;
    
    // Pop b first, then a (for a - b, a / b, etc.)
    int b = pop();
    int a = pop();

    // If pop() caused an error, stop
    if (has_error) return;

    switch(op) {
        case '+': push(a + b); break;
        case '-': push(a - b); break;
        case '*': push(a * b); break;
        case '^': push((int)pow(a, b)); break;
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

// Error Function
void error(const char *msg) {
    if (has_error) return; // Only report the first error
    
    printf("%s\n", msg);
    has_error = 1;
    
    // Per requirement, consume the rest of the line to stop processing
    char c;
    while ((c = input()) != '\n' && c != EOF) { /* consume */ }

    // Reset stack for next line
    top = -1;
}

%}

%%

// Ignore whitespace
[ \t]+          { /* ignore */ }

// A number (integer, possibly negative)
"-?"[0-9]+      { push(atoi(yytext)); }

// Operators
"+"             { bin_op('+'); }
"-"             { bin_op('-'); }
"*"             { bin_op('*'); }
"/"             { bin_op('/'); }
"%"             { bin_op('%'); }
"^"             { bin_op('^'); }

// End of line - check results
\n              {
                  if (!has_error) {
                      if (top == 0) {
                          // Success: exactly one item on stack
                          printf("= %d\n", stack[top]);
                      } else if (top > 0) {
                          // Error: too many items left 
                          error("Error: Too few operators");
                      }
                      // if top == -1, it was an empty line; do nothing
                  }
                  
                  // Reset for the next line
                  has_error = 0;
                  top = -1;
                }

// Any other symbol is an error 
.               { 
                  char msg[100];
                  sprintf(msg, "Error: Bad symbol \"%s\"", yytext);
                  error(msg);
                }

%%

int yywrap() {
    return 1;
}

int main(int argc, char *argv[]) {
    printf("RPN Calculator ready. Enter expressions.\n");
    yylex();
    return 0;
}