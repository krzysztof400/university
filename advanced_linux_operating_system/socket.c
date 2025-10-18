#include <sys/socket.h>

int main(){
    int sock;
    //connected
    sock = socket(AF_INET, SOCK_STREAM, 0);

    //offline
    sock = socket(AF_INET, SOCK_DGRAM, 0);
}