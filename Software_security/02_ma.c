//memory allocation, how to not free pointers
#include <stdio.h>
#include <stdlib.h>


int f(int a, int b){
    int *p1, *p2, *p3;
    p1 = (int *) malloc(sizeof(int));
    *p1 = a;
    p2=p1;
    if(a > b){
        free(p1);
        p1 = NULL;
    }
    p3 = (int *) malloc (sizeof(int));
    *p3 = b;
    printf("p3: %p, *p2: %d\n", p3, *p2);
    free(p1);
    free(p3);
}

int main(int argc, char* argv[]){
    (void) argc;
    (void) argv;
    f(1,2);
    f(5,3);
    return 0;
}

/*after freeing p1 pointer during second functoin execution (in the 'if'), we free memory allocated to p1 pointer, 
which is also used by p2 pointer. The problem is that in 17th line we use p2 pointer once again, 
we read to that p2 pointer points, but beetween freeing p1 pointer and reading p2 pointer that memory writen*/