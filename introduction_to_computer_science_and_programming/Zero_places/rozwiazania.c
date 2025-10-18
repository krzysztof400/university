#include <math.h>
#include "header.h"

double rozwiazania(double a, double b, double eps)
{
    double i = (a+b)/2;
    if(fabs(b-a) <= 2*eps){
        return i;
    }
    if(f(i)*f(a) <= 0){
        return rozwiazania(a, i, eps);
    }
    return rozwiazania(i, b, eps);
}