#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/user.h>
#include <sys/reg.h>
#include "__SCLIST64_H__.h"

#define MAX_SYSCALL 314

int main(int argc, char *argv[]){
    if(argc < 2){
        printf("Usage: %s <syscall number>\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();
    switch(pid){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            if(ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1){
                perror("ptrace");
                exit(1);
            }

            kill(getpid(), SIGSTOP);

            execvp(argv[1], argv + 1);
            exit(1);
        default:
            waitpid(pid, NULL, 0);
            long syscall_count[MAX_SYSCALL] = {0};
            int in_syscall = 0;

            while(1){
                if(ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1){
                    perror("ptrace");
                    exit(1);
                }
                int status;
                long syscall_no;
                struct user_regs_struct regs;

                waitpid(pid, &status, 0);
                if(WIFEXITED(status)){
                    break;
                }

                if (!in_syscall) {
                    syscall_no = ptrace(PTRACE_PEEKUSER, pid, sizeof(long) * ORIG_RAX, NULL);

                    if (syscall_no < MAX_SYSCALL) {
                        syscall_count[syscall_no]++;
                    }
                    in_syscall = 1;
                } 
                else {
                    in_syscall = 0;
                }
            }
            fprintf(stderr, "\nSystem call count:\n");
            for (int i = 0; i < MAX_SYSCALL; i++) {
                if (syscall_count[i] > 0) {
                    fprintf(stderr, "Syscall %s: %ld times\n",syscall_list[i], syscall_count[i]);
                }
            }
    }
}