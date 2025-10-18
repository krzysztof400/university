#include <stdbool.h>
#include <stdio.h>

bool match(char* pattern, char* str) {
    if (*pattern == '\0' && *str == '\0') 
    {
        return true;
    }
    if (*pattern == '*') 
    {
        return match(pattern + 1, str) || (*str != '\0' && match(pattern, str + 1));
    }
    if (*pattern == '\0') 
    {
        return false;
    }
    if (*pattern == *str || *pattern == '?') 
    {
        return match(pattern + 1, str + 1);
    }
    return false;
}

int main() {
    char pattern[50];
    char str[50];

    printf("Podaj wzor: ");
    scanf("%s", &pattern);
    printf("Podaj lancuch: ");
    scanf("%s", &str);

    if(match(pattern, str))
    {
        printf("napisy sa zgodne\n\n");
    }
    else
    {
        printf("napisy nie sa zgodne\n\n");
    }
    return 0;
}
