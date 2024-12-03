#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> // Wait for the child process to end

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
    }
    else{
        // Parent process
        printf("Father: \n");
        printf("Father PID: %d\n", getpid());
        printf("Son PID: %d\n", pid);
        for(int i = 0; i < 30; i++){
            printf("Father: %d\n", i);
            sleep(1);
        }
        WAIT(NULL); // Wait for the child process to end
        printf("Father end\n");
    }
}