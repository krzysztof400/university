#include <stdio.h>
#include <string.h>

void hex_arithmetic();
void binary_operations();

int main(){
    hex_arithmetic();
    binary_operations();
    return 0;
}

void hex_arithmetic(){
    float x = 0xfbfffff;
    if (x = x){
        printf("True\n");
    } else {
        printf("False\n");
    }

    float result = x-x;

    if (result == 0){
        printf("True\n");
    } else {
        printf("False\n");
    }
}

void binary_operations(){
    float a = (1ul<<53) + 1ul; // 2^53 + 1, firslty we perform binary operation move 1 to the left 53 times which is 2^53, then we add 1
    float b = 9007199254740992.0;
    float c = b;

    if (a == b){
        printf("a = b\n");
    }
    if (b == c){
        printf("b = c\n");
    }
    if (a == c){
        printf("a = c\n");
    }
}