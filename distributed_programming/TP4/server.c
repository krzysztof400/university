#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define MAX_BUFFER 1024
#define MAX_CLIENTS 10
#define SOCKET_PATH "/tmp/my_unix_socket"

void server(char *my_socket){
    char send_buffer[MAX_BUFFER];
    char recv_buffer[MAX_BUFFER];

    // Create the listening socket with AF_UNIX
    int listening_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listening_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    } else {
        printf("Socket: %d\n", listening_socket);
    }

    // Define server address for AF_UNIX
    struct sockaddr_un server_address, client_address;
    memset(&server_address, 0, sizeof(struct sockaddr_un));
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, SOCKET_PATH);

    // Remove any existing socket file
    unlink(SOCKET_PATH);

    // Bind the socket to the address
    if (bind(listening_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(listening_socket, MAX_CLIENTS) == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Accept incoming connection
    socklen_t size = sizeof(client_address);
    int service_socket = accept(listening_socket, (struct sockaddr*)&client_address, &size);
    if (service_socket == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Receive a message from the client
    read(service_socket, recv_buffer, sizeof(recv_buffer));
    printf("Received message: %s\n", recv_buffer);

    // Send a message to the client
    strcpy(send_buffer, "OK");
    write(service_socket, send_buffer, strlen(send_buffer) + 1);

    // Clean up and close the sockets
    close(service_socket);
    close(listening_socket);
    unlink(SOCKET_PATH);  // Clean up the socket file
}

void client(char *my_socket, char *server_socket, char *message){
    // Create the socket
    int client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // Define server address for AF_UNIX
    struct sockaddr_un server_address;
    memset(&server_address, 0, sizeof(struct sockaddr_un));
    server_address.sun_family = AF_UNIX;
    strcpy(server_address.sun_path, SOCKET_PATH);

    // Connect to the server
    if (connect(client_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    // Send a message to the server
    write(client_socket, message, strlen(message) + 1);
    pid_t pid = getpid();
    write(client_socket, &pid, sizeof(pid_t));

    // Receive a message from the server
    char recv_buffer[MAX_BUFFER];
    read(client_socket, recv_buffer, sizeof(recv_buffer));

    // Clean up and close the socket
    close(client_socket);
}

int main(int argc, char *argv[]) {

    // Managing the arguments and assigning the server or client function
    if (argc < 2) {
        printf("Usage: %s <option>\n", argv[0]);
        printf("Options:\n");
        printf("-s - server\n");
        printf("-c - client\n");
        return 1;
    }
    
    if (strcmp(argv[1], "-s") == 0) {
        printf("Chosen server option\n");
        if(argc != 3){
            printf("Usage: %s <option> <my_socket>\n", argv[0]);
            return 1;
        }
        server(argv[2]);
    } 
    else if (strcmp(argv[1], "-c") == 0) {
        printf("Chosen client option\n");
        if(argc != 5){
            printf("Usage: %s <option> <my_socket> <server_socket> <message>\n", argv[0]);
        }
        client(argv[2], argv[3], argv[4]);
    }
    else {
        printf("Invalid option\n");
    }

    return 0;
}