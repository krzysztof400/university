#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <fcntl.h>

#define MAX_COMMAND_SIZE 256
#define MAX_ARGS 64

int split(char *str, char **word);
void execute(char **args);
void special_symbols(char **args);

int main(){
    char command[MAX_COMMAND_SIZE];
    char *args[MAX_ARGS];
    while(1){
        printf("shell> ");
        fgets(command, MAX_COMMAND_SIZE, stdin);

        command[strcspn(command, "\n")] = 0;

        if(strcmp(command, "exit") == 0){
            break;
        }

        int argc = split(command, args);
        args[argc] = NULL;

        execute(args);
    }
}

int split (char *str, char **word){
    int i = 0;
    char *token = strtok(str, " ");

    while(token != NULL){
        word[i] = token;
        token = strtok(NULL, " \f\n\r\t\v");
        i++;
    }

    return i;
}

void execute(char **args){
    pid_t pid;
    int saved_stdin = dup(0);
    int saved_stdout = dup(1);
    int saved_stderr = dup(2);

    for(int i = 0; args[i] != NULL; i++){
        if(strcmp(args[i], "<") == 0){
            args[i] = NULL;
            int fd = open(args[i+1], O_RDONLY);
            dup2(fd, 0);
            close(fd);
        }
        else if(strcmp(args[i], ">") == 0){
            args[i] = NULL;
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, 1);
            close(fd);
        }
        else if(strcmp(args[i], ">>") == 0){
            args[i] = NULL;
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            dup2(fd, 1);
            close(fd);
        }
        else if(strcmp(args[i], "2>") == 0){
            args[i] = NULL;
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(fd, 2);
            close(fd);
        }
        else if(strcmp(args[i], "2>>") == 0){
            args[i] = NULL;
            int fd = open(args[i+1], O_WRONLY | O_CREAT | O_APPEND, 0644);
            dup2(fd, 2);
            close(fd);
        }
    }

    switch(pid = fork()){
        case -1:
            perror("failed to fork\n");
            exit(1);
            break;
        case 0:
            execvp(args[0], args);
            printf("failed to execute command\n");
            exit(1);
            break;
        default:
            int status;
            waitpid(pid, &status, 0);
            break;
    }

    dup2(saved_stdin, 0);
    dup2(saved_stdout, 1);
    dup2(saved_stderr, 2);
    close(saved_stdin);
    close(saved_stdout);
    close(saved_stderr);
}

void special_symbols(char **args){
    char *command = strtok(args, "&;|");
    char *separator;
    int last_status = 0;

    while (command != NULL) {
        separator = strtok(NULL, "&;");

        if (separator == NULL || *separator == ';') {
            if (fork() == 0) {
                execute(command);
                exit(0);
            }
        } 
        else if (*separator == '&') {
            last_status = execute(command);
        } 
        else if (strncmp(separator, "&&", 2) == 0) {
            if (last_status == 0) {
                last_status = execute(command);
            }
        } 
        else if (strncmp(separator, "||", 2) == 0) {
            if (last_status != 0) {
                last_status = execute(command);
            }
        }

        command = strtok(NULL, "&;");
    }
}