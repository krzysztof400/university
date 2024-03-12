#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "header.h"

bool wzglpierwsze(int num, int i){
    while (i != 0 && num != 0)
        if(num > i){
            num = num%i;
        }
        else{
            i = i%num;
        }
    if (i == 1 || num == 1){
        return true;
    }
    else{
        return false;
    }
}