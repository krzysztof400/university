#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SECRET_SIZE_MAX 10;

int main(int argc, char* argv[]){
    (void) argc;

    char *secret = malloc(SECRET_SIZE_MAX);
    strncpy(secret, argv[1], SECRET_SIZE_MAX-1);
        //we didnt use strcpy, because strncpy stops after (third arg) number of bytes. We also use argv[1] without being sure it was given, thats bad
    size_t secret_size = strlen(secret);
    if(secret_size > SECRET_SIZE_MAX / 2){
        return 1;
    }
    else{
        secret = (char *)realloc(secret, secret_size*2);
    }

    /*More code here*/

    return 0;
}

/*
realloc(*pointer, int size){
    allocating_new_memory_for_given_size();
    assigning_pointer_to_new_space();

    //the problem is that realloc is leaving our secret for everyone to read from it, 
    //after freeing previous memory the content of it is still in memory we just lost the pointer for it and made it free for everyone
}
*/