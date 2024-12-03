#include <stdio.h>
#include <string.h>

#define PASSWORD "Z7B4h" // Change this to any password you like (max 5 characters)

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <password>\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], PASSWORD) == 0) {
        printf("Access granted\n");
    } else {
        printf("Bad password\n");
    }

    return 0;
}
