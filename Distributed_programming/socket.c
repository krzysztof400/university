#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>

// #define SOCK_DGRAM      // sends structeded messaged
// #define SOCK_STREAM     // sends byte streams


// domains of the sockets
// AF_INET     // IPv4 - 32 bytes of adress 
// AF_INET6    // IPv6 - smaller adress
// AF_INET     // by default IPv4 - 32 bytes of adress

// IP adress - 4 bytes seperated by points, sended with mask (also 4 bytes). First three bytes are reserved for the network, the last one is for the host
// DNS - translates URL to IP adresses

// IP frame - begining with header containing the source and destination IP adresses, the protocol used and the size of the frame. The data is after the header



int main() {
    int socket_discriptor = socket(AF_INET, SOCK_STREAM, 0);            // function socket library sys/socket.h, af_inet is the domain of the socket in netinet library
                                                                        // sock_stream is the type of the socket, 0 is the protocol used
    
    int socket_discriptor2 = socket(AF_INET, SOCK_STREAM, 0);     
    if(socket_discriptor == -1 || socket_discriptor2 == -1){
        perror("socket");
        return 1;
    }
    else{
        printf("Socket: %d\n", socket_discriptor);
        printf("Socket: %d\n", socket_discriptor2);
    }

    close(socket_discriptor);
    close(socket_discriptor2);
}

// binding socket to an adress