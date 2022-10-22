#include <iostream>
#include <cstdlib>
#include <unistd.h>
#ifdef __linux__
#include <sys/socket.h>
#include <netiinet/in.h>
#elif _WIN32
#include <Winsock2.h>
#else

#endif


int main() {
    std::cout << "Hello, World!" << std::endl;

    // Create a socket (IPv4, TCP)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd == -1 ) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit( EXIT_FAILURE );
    }

    // Listen to port 9999 on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.S_un.S_addr = INADDR_ANY;
    sockaddr.sin_port = htons( 9999 ); // Hton converts number to network byte order

    if( bind( sockfd, ( struct sockaddr* )&sockaddr, sizeof( sockaddr ) ) < 0 ) {
        std::cout << "Failed to bind to port 9999. erno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}
