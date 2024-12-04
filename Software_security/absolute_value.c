#include <stdio.h>
#include <stdlib.h>

void their_abs();
void my_abs();

int main(){
    their_abs();
    my_abs();
    return 0;
}

void their_abs(){
    int value;
    scanf("%d", &value);
    int result = abs(value);
    printf("The absolute value of %d is: %d\n", value, result);
}

void my_abs(){
    int value;
    scanf("%d", &value);
    int result = value;
    if(value < 0){
        result = -value;
    }
    printf("The absolute value of %d is: %d\n",result, value);
}

// doesnt work for INT_MIN -2147483648, because of overflow