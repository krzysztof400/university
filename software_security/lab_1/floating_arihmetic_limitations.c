#include <stdio.h>

#define MAX 100

int main(){
    double previous = 0.1;
    double new;
    printf("previous = %f\n", previous);
    for (int i =0; i<MAX; i++){
        new = 12*previous - 1.1;
        printf("new = %f\n", new);
        previous = new;
    }
    return 0;
}

// after 10 stable iterations, the value of new converges from 0.1 it is due to
// floating point arithematic error
// Doubles are stored (-1)^s * M * 2^E, where s is sign digit and E is scale
// only finite element od digits can be stored using double, thus it leads to rounding error
// which in iterative tasks leads to accumulation of error

// Instead we can:
// 1. use libraries like mpfr, fg use mpfr_set_d 
// 2. use long double
// 3. use integer arithematic
// 4. use fixed point arithematic
/*
#define SCALE 1000000
int previous = 100000;  // represents 0.1 as 100000 / SCALE
int new;
new = 12 * previous - 1100000;
*/
// 5. you can also reformulate the problem to avoid the error, fg create if that doesnt
// allow for operations on the same numnbers