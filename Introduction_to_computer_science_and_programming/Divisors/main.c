#include <stdio.h>
#include <stdbool.h>

int isprime(int n){
    int i = 2;
    for(i; i < n; i++){
        if(n%i == 0){
            return 0;
        }
    }
    return 1;
}

int main(){
    int n;
    scanf("%i", &n);
    int i = 2;
    for(i; i <= n; i++){
        while(isprime(i) == 1 && n%i == 0){
            printf("%i\n", i);
            n = n/i;
        }
    }
}



