#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 256
#define MAX_CLIENTS 10

// Array to store client sockets
int clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to broadcast a message to all clients except the sender
void broadcast_message(char *message, int sender_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i] != sender_socket) {
            send(clients[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Function to handle communication with a client
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (1) {
        // Receive message from client
        bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("Client disconnected or error receiving data\n");
            break;
        }
        buffer[bytes_received] = '\0';
        printf("Received from client: %s\n", buffer);

        // Broadcast the message to other clients
        broadcast_message(buffer, client_socket);
    }

    close(client_socket);

    // Remove client from the clients array
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i] == client_socket) {
            clients[i] = clients[client_count - 1];
            client_count--;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Thread function to handle a client
void *client_handler(void *arg) {
    int client_socket = *(int *)arg;
    handle_client(client_socket);
    free(arg);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("You have to insert an option\n");
        return 1;
    }

    char *option = argv[1];

    // Server option
    if (strcmp(option, "-s") == 0) {
        printf("Creating a server\n");

        // Create a socket
        int server_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (server_socket == -1) {
            printf("Failed to create socket\n");
            return 1;
        }

        // Define the server address
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(0); // Bind to any available port
        server_address.sin_addr.s_addr = INADDR_ANY;

        // Bind the server
        if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
            printf("Failed to bind to address\n");
            return 1;
        }

        // Get the port number
        socklen_t len = sizeof(server_address);
        if (getsockname(server_socket, (struct sockaddr *)&server_address, &len) == -1) {
            printf("Failed to get socket name\n");
            return 1;
        }

        printf("Server listening on port: %d\n", ntohs(server_address.sin_port));
        printf("Server IP: %s\n", inet_ntoa(server_address.sin_addr));

        // Listen for incoming connections
        listen(server_socket, 10);
        printf("Waiting for connections...\n");

        while (1) {
            // Accept a new client connection
            int *client_socket = malloc(sizeof(int));
            // Set variable for client socket
            *client_socket = accept(server_socket, NULL, NULL);
            if (*client_socket == -1) {
                printf("Failed to accept connection\n");
                free(client_socket);
                continue;
            }
            printf("Client connected.\n");

            // Add the client to the clients array, mutex is used to preserve safety and disallow simantanious manipulation of data
            pthread_mutex_lock(&clients_mutex);
            clients[client_count++] = *client_socket;
            pthread_mutex_unlock(&clients_mutex);

            // Create a new thread to handle the client
            pthread_t thread;
            pthread_create(&thread, NULL, client_handler, client_socket);
            pthread_detach(thread);
        }

        // Close the server socket
        close(server_socket);
    }
    
    // Writer option
    else if (strcmp(option, "-w") == 0) {
        printf("Chosen writer option\n");

        if (argc < 5) {
            printf("You have to submit IP, port and nick\n");
            return 1;
        }

        char *ip = argv[2];
        int port = atoi(argv[3]);
        char *nick = argv[4];

        // Create a socket
        int client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            printf("Failed to create socket\n");
            return 1;
        }

        // Define the server address
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);
        inet_pton(AF_INET, ip, &server_address.sin_addr);

        // Connect to the server
        if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
            printf("Connection failed\n");
            return 1;
        }

        printf("Connected to server. Type messages and press Enter to send:\n");

        char buffer[BUFFER_SIZE];
        while (1) {
            // Get the message from the user
            printf("You: ");
            fgets(buffer, sizeof(buffer), stdin);

            // Get the current time
            time_t now = time(NULL);
            struct tm *timeinfo = localtime(&now);

            // Create a string with the time, nick and message
            char time_str[BUFFER_SIZE] = {0};
            strftime(time_str, sizeof(time_str), "%H:%M:%S", timeinfo);
            strcat(time_str, " ");
            strcat(time_str, nick);
            strcat(time_str, ": ");
            strcat(time_str, buffer);

            // Send the message to the server
            send(client_socket, time_str, strlen(time_str), 0);

            // Clear the buffer
            memset(buffer, 0, sizeof(buffer));
        }

        // Close the client socket
        close(client_socket);
    } 
    // Reader option
    else if (strcmp(option, "-r") == 0) {
        printf("Chosen reader option\n");

        // Check if the user has provided IP and port
        if (argc < 4) {
            printf("You have to submit IP and port\n");
            return 1;
        }

        char *ip = argv[2];
        int port = atoi(argv[3]);

        // Create a socket
        int client_socket = socket(AF_INET, SOCK_STREAM, 0);
        if (client_socket == -1) {
            printf("Failed to create socket\n");
            return 1;
        }

        // Define the server address
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(port);
        inet_pton(AF_INET, ip, &server_address.sin_addr);

        // Connect to the server
        if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
            printf("Connection failed\n");
            return 1;
        }

        char buffer[BUFFER_SIZE];
        while (1) {
            // Receive messages from the server
            int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) {
                printf("Server disconnected or error receiving data\n");
                break;
            }

            buffer[bytes_received] = '\0';
            printf("%s\n", buffer);
        }

        // Close the client socket
        close(client_socket);
    } 
    // Unknown option
    else {
        printf("Unknown option: %s\n", option);
    }

    return 0;
}