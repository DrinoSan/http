//
// Created by becirbeg on 22.10.2022.
//

#include "simpleHttpServer.h"
#include <sys/socket.h>
#include <cstring>
        
bool simpleHttpServer::startServer(std::string ipAddr, int64_t port)
{
    // Create a socket (IPv4, TCP)
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if( sockfd == -1 ) {
        std::cout << "Failed to create socket. errno: " << errno << std::endl;
        exit( EXIT_FAILURE );
    }

    // Listen to port 9999 on any address
    sockaddr_in sockaddr;
    sockaddr.sin_family = AF_INET;
    if(ipAddr != "") {
        inet_pton(AF_INET, ipAddr.c_str(), &(sockaddr.sin_addr));
    }
    else {
        sockaddr.sin_addr.s_addr = INADDR_ANY;
    }
    sockaddr.sin_port = htons( port ); // Hton converts number to network byte order

    if( bind( sockfd, ( struct sockaddr* )&sockaddr, sizeof( sockaddr ) ) < 0 ) {
        std::cout << "Failed to bind to port " << port << ". errno: " << errno << std::endl;
        return false;
    }

    std::cout << "Socket FileDeskriptor: " << sockfd << " " << std::endl;

    // Start listening. Hold at most 10 connection in the queue
    if(listen(sockfd, 10) < 0) {
        std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Grab connection from the queue
    auto addrlen = sizeof(sockaddr);
    int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*)&addrlen);
    if(connection < 0) {
        std::cout << "Failed to grab connection. errno: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }

    // Read from the connection
    char buffer[200];
    auto bytesRead = read(connection, buffer, 200);
    parseRequest(buffer);
    std::cout << "-----------------" << std::endl;
    std::cout << "The message was: " << buffer;
    std::cout << "-----------------" << std::endl;

    // Send a message to the connection
    std::string response = "Good talking to you\n";
    send(connection, response.c_str(), response.size(), 0);

    // Close the connections
    close(connection);
    close(sockfd);
    return true;
}

void simpleHttpServer::parseRequest(const char*  buffer) {
    std::string msg = "GET / HTTP/1.1\r\n"
                    "Host: 192.241.213.46:6880\r\n"
                    "Upgrade-Insecure-Requests: 1\r\n"
                    "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                    "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/602.4.8 (KHTML, like Gecko) Version/10.0.3 Safari/602.4.8\r\n"
                    "Accept-Language: en-us\r\n"
                    "Accept-Encoding: gzip, deflate\r\n"
                    "Connection: keep-alive\r\n\r\n";


    const char* buf = msg.c_str();
    const char* msg_end = "\r";
	const char* new_line = "\n";

    const char* head = buf;
    const char* tail = buf;
    const char* runner = buf;

    // Find request type
    while( tail != msg_end && *tail != ' ' ) ++tail;
    http_request["Type"] = std::string(head, tail);

    // We need to increment tail because it is currently on the whitspace
    head = tail++;

    // Find path
    while( tail != msg_end && *tail != ' ' ) ++tail;
    http_request["Path"] = std::string(head, tail);

    // Find HTTP version
    while( tail != msg_end && *tail == ' ' ) ++tail;
	head = tail;

	while( tail != msg_end && *tail != '\r' ) ++tail;
	http_request["Version"] = std::string( head, tail );

    if( tail != msg_end) ++tail; // skip '\r'
    if( tail != new_line) ++tail; // skip '\n'

	head = tail;

    while( *tail != '\0' ) {
        // Find key
        const char* colon = strchr(head, ':');
        if( colon == NULL ) {
            std::cout << "Shit header" << std::endl;
        }
        std::string type(head, colon);
        
        std::cout << "Type: " << type << std::endl;

        while( *tail != '\r' ) ++tail;
        // Find value
        const char* value = colon+1;
        std::cout << "SANDDDDDDDDDDDDDDDDDD: " << *colon << std::endl;
        std::string val(value, tail);

        std::cout << "Val: " << val << std::endl;
        head = tail+2;

    }

    //for( const auto& [key, val] : http_request ) {
        //std::cout << "HEADER: " << key << " : " << val << std::endl;
    //}

}
