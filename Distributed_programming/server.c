#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080
#define MAX_CLIENTS 10
#define MAX_BUFFER 1024

void server(){
    char send_buffer[MAX_BUFFER];
    char recv_buffer[MAX_BUFFER];

    int listening_socket = socket(AF_INET, SOCK_STREAM, 0);    
    if(listening_socket == -1){
        perror("socket");
    }
    else{
        printf("Socket: %d\n", listening_socket);
    }

    struct sockaddr_in server_adress, client_adress;
        server_adress.sin_family = AF_INET;
        server_adress.sin_addr.s_addr = INADDR_ANY;
        server_adress.sin_port = htons(PORT);

    bind(listening_socket, (struct sockaddr*)&server_adress, sizeof(server_adress));

    listen(listening_socket, MAX_CLIENTS);

    int service_socket = accept(listening_socket, (struct sockaddr*)&client_adress, &size); // as much as the number of clients

    strcpy(send_buffer, "Hello from server!");

    strcpy(send_buffer, "Hello from server!");

    write(service_socket, send_buffer, sizeof(send_buffer));
    read(service_socket, recv_buffer, sizeof(recv_buffer));

    printf("message: %s\n", recv_buffer);

    close(service_socket);
    close(listening_socket);
}

void client(){
    char send_buffer[MAX_BUFFER];
    char recv_buffer[MAX_BUFFER];

    int socket_discriptor = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_discriptor == -1){
        perror("socket");
    }
    else{
        printf("Socket: %d\n", socket_discriptor);
    }

    struct sockaddr_in server_adress, client_adress;
        server_adress.sin_family = AF_INET;

    strcpy(send_buffer, "Hello from client!");

    send_buffer = "Hello from client!";

    write(socket_discriptor, send_buffer, sizeof(send_buffer));
    read(socket_discriptor, recv_buffer, sizeof(recv_buffer));

    printf("message: %s\n", recv_buffer);

    close(socket_discriptor);
}

int main (int argc, char *argv[]) {
    if (argc == 1){
        server();
    }
    else{
        client();
    }
    return 0;
}