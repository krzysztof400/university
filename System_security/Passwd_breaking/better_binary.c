#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_CHAR 100

char password[MAX_CHAR];

int my_strcmp(char *str1, char *str2)
{
    int res = 0;
    unsigned int i = 0;

    for (i = 0; str1[i] != '\0' && str2[i] != '\0'; ++i) {
        if (str1[i] == str2[i]) {
            sleep(1);
        }
    }
    res = str1[i] - str2[i];
    return res;
}


int main()
{
    char input[MAX_CHAR];
    FILE *file;

    /* Get the password */
    if ((file = fopen("/opt/.vault","r")) == NULL){
        printf("Error : No file encountered!");
        exit(1);
    }
    fgets(password, MAX_CHAR, file);
    /* remove \n */
    password[strcspn(password, "\n")] = '\0';
    fclose(file);

    printf( "Please enter a password : ");
    scanf("%s",input);

    /* Comparing strings */
    if(my_strcmp(password, input) != 0){
        printf( "\nBad password.\n");
    }
    else {
        printf( "\nAccess granted.\n");
    }
    return 0;
}
