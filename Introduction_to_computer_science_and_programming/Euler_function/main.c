#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "header.h"

int main(){
    long int n;
    printf("podaj liczbe: ");
    scanf("%ld", &n);

    int i = 1;
    int suma = 0;
    for(i; i < n; i++)
        if(wzglpierwsze(i, n)){
            suma += 1;
        }
    printf("%u\n\n", suma);
}