#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void handle_sigusr1() {
    printf("Received SIGUSR1, breaking the loop.\n");
    exit(0);
}

int main(){
    pid_t pid = fork();
    if(pid == -1){
        // Error
        perror("fork");
        exit(1);
    }
    if(pid == 0){
        // Child process
        int id;
        printf("Son:\n");
        printf("Son PID: %d\n", getpid());
        printf("Father PID: %d\n", getppid());
        scanf("%d", &id);
        if(id == 0){
            kill(getppid(), SIGUSR1);
        }
    }
    else{
        // Parent process
        int i = 0;
        printf("Father: \n");
        printf("Father PID: %d\n", getpid());
        printf("Son PID: %d\n", pid);
        signal(SIGUSR1, handle_sigusr1);
        while(1){
            printf("Father: %d\n", i);
            i++;
            sleep(1);
        }
        printf("Father end\n");
    }
}