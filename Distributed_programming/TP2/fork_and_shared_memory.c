#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

void handle_signal(int sig) {
    printf("Received signal %d\n", sig);
    exit(0);
}

int* create_shared_memory() {
    int fd = shm_open("my_shm", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(fd == -1) {
        perror("shm_open");
        exit(1);
    }
    if(ftruncate(fd, sizeof(int)) == -1) {
        perror("ftruncate");
        exit(1);
    }
    int* shared_memory = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(shared_memory == MAP_FAILED) {
        perror("mmap");
        exit(1);
    }
    *shared_memory = 0;
    return shared_memory;
}

void cleanup_shared_memory() {
    shm_unlink("my_shm");
}

int main(){
    pid_t pid = fork();

    int* shared_memory = create_shared_memory();

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
            *shared_memory = 0;
        }
    }
    else{
        // Parent process
        printf("Father: \n");
        printf("Father PID: %d\n", getpid());
        printf("Son PID: %d\n", pid);
        while(1){
            printf("Father: %d\n", *shared_memory);
            (*shared_memory)++;
            sleep(1);
            if(*shared_memory == 0) {
                break;
            }
        }
        printf("Father end\n");
    }

    cleanup_shared_memory();
}