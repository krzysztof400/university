#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

void reciever(char *local_port){
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(atoi(local_port));
        server_address.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    // Get the port number
    socklen_t len = sizeof(server_address);
    if (getsockname(server_socket, (struct sockaddr *)&server_address, &len) == -1) {
        printf("Failed to get socket name\n");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port: %s\n", local_port);
    printf("Server IP: %s\n", inet_ntoa(server_address.sin_addr));

    // Listen for incoming connections
    listen(server_socket, 10);
    printf("Waiting for connections...\n");

    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);

    int client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_size);
    if (client_socket == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Client connected\n");

    char recv_buffer[1024];
    char send_buffer[1024];
    read(client_socket, recv_buffer, sizeof(recv_buffer));
    printf("Received message: %s\n", recv_buffer);

    // Send a message to the client
    strcpy(send_buffer, "OK");
    write(client_socket, send_buffer, strlen(send_buffer) + 1);

    // Close the client socket
    close(client_socket);
}

void sender(char *local_port, char *name_of_distance_machine, char *name_of_distance_port){
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(name_of_distance_port));
    inet_pton(AF_INET, name_of_distance_machine, &server_address.sin_addr);

    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    char send_buffer[1024];
    scanf("%s", send_buffer);
    write(client_socket, send_buffer, strlen(send_buffer) + 1);

    char recv_buffer[1024];
    read(client_socket, recv_buffer, sizeof(recv_buffer));
    printf("Received message: %s\n", recv_buffer);
    
    // Close the client socket
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
    
    if (strcmp(argv[1], "-r") == 0) {
        printf("Chosen server option\n");
        if(argc != 3){
            printf("Usage: %s <option> <local_port>\n", argv[0]);
            return 1;
        }
        reciever(argv[2]);
    } 
    else if (strcmp(argv[1], "-s") == 0) {
        printf("Chosen client option\n");
        if(argc != 5){
            printf("Usage: %s <option> <local_port> <name_of_distance_machine> <name_of_distance_port>\n", argv[0]);
        }
        sender(argv[2], argv[3], argv[4]);
    }
    else {
        printf("Invalid option\n");
    }

    return 0;
}