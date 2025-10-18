#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "funs.h"


int main (){
    char napis[80];
    scanf("%s", napis);
    if(palindrom(napis)){
        printf("wyraz jest palindromem\n");
    }
    else{
        printf("wyraz nie jest palindromem\n");
    }
}