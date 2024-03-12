#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "funs.h"

bool palindrom (char napis[]){
    int n = strlen(napis);
    int i;
    int j;
    for(i = 0, j = n - 1; i < j; i++, j--){
        if(napis[i] != napis[j]){
            return false;
        }
    }
    return true;
}