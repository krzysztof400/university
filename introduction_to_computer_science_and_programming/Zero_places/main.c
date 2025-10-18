#include <math.h>
#include <stdio.h>
#include "header.h"

int main(){
    double a;
    double b;
    double eps;
    printf("Podaj a i b, dla ktorego f(a)*f(b)<0 i a < b:\na: ");
    scanf("%lf", &a);
    printf("b: ");
    scanf("%lf", &b);

    if(a > b){
        printf("blad, podaj takie wartosci, ze a < b");
        return 0;
    }
    if(f(a)*f(b) >= 0){
        printf("blad, podaj takie wartosci, ze f(a)*f(b)<0");
        return 0;
    }
    printf("\n\nPodaj dokladnosc z jaka znajadziemy miejsce zerowe |nasz wynik - miejsce zerowe| < eps\neps: ");
    scanf("%lf", &eps);

    printf("%lf", rozwiazania(a, b, eps));
    return 0;
}
