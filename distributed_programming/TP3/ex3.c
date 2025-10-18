#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 100
char buffer[BUFFER_SIZE];
char file_name[] = "text.txt";

pthread_mutex_t lock1;
pthread_mutex_t lock2;

// function executed by thread 1
void* read_file(void* arg) {
    FILE* fp = fopen(file_name, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file: %s\n", file_name);
        exit(EXIT_FAILURE);
    }

    while (1) {
        pthread_mutex_lock(&lock2); // Wait for thread 2 to print the buffer

        if (fscanf(fp, "%s", buffer) == EOF) {
            strcpy(buffer, "EOF");
            pthread_mutex_unlock(&lock1); // Allow thread 2 to process "EOF"
            break;
        }

        pthread_mutex_unlock(&lock1); // Allow thread 2 to print the word
    }

    fclose(fp);
    return NULL;
}

// function executed by thread 2
void* print_buffer(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock1); // Wait for thread 1 to fill the buffer

        if (strcmp(buffer, "EOF") == 0) {
            pthread_mutex_unlock(&lock2); // Allow thread 1 to exit
            break;
        }

        printf("Word: %s\n", buffer);
        pthread_mutex_unlock(&lock2); // Allow thread 1 to continue reading
    }

    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    pthread_mutex_init(&lock1, NULL);
    pthread_mutex_init(&lock2, NULL);

    pthread_mutex_lock(&lock1);

    pthread_create(&thread1, NULL, read_file, NULL);
    pthread_create(&thread2, NULL, print_buffer, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    pthread_mutex_destroy(&lock1);
    pthread_mutex_destroy(&lock2);

    return EXIT_SUCCESS;
}
