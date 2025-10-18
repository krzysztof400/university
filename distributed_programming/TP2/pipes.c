#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAX_SIZE 100

void handle_sigusr1() {
    printf("Received SIGUSR1, breaking the loop.\n");
    exit(0);
}

int main() {
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(1);
    }
    pid_t pid = fork();
    if (pid == -1) {
        // Error
        perror("fork");
        exit(1);
    }
    printf("Pipe: %d %d\n", pipefd[0], pipefd[1]);

    if (pid == 0) {
        // Child process
        close(pipefd[0]); // We dont need the read we will be sending data
        char message[MAX_SIZE];
        printf("Son:\n");
        printf("Son PID: %d\n", getpid());
        printf("Father PID: %d\n", getppid());
        scanf("%s", message);
        write(pipefd[1], message, sizeof(message));
        close(pipefd[1]);
    } else {
        // Parent process
        int i = 0;
        printf("Father: \n");
        printf("Father PID: %d\n", getpid());
        printf("Son PID: %d\n", pid);

        close(pipefd[1]);
        char message[MAX_SIZE];
        read(pipefd[0], message, sizeof(message));

        printf("Father end\n");
    }

    return 0;
}