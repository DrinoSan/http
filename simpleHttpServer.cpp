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
        exit(EXIT_FAILURE);
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
    parseRequest(std::string(buffer));
    std::cout << "The message was: " << buffer;

    // Send a message to the connection
    std::string response = "Good talking to you\n";
    send(connection, response.c_str(), response.size(), 0);

    std::cout << "The Type was: " << http_request["Type"] << std::endl;
    // Close the connections
    close(connection);
    close(sockfd);
    return true;
}
 //char * msg= "GET / HTTP/1.1\r\n"
                //"Host: 192.241.213.46:6880\r\n"
                //"Upgrade-Insecure-Requests: 1\r\n"
                //"Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                //"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) AppleWebKit/602.4.8 (KHTML, like Gecko) Version/10.0.3 Safari/602.4.8\r\n"
                //"Accept-Language: en-us\r\n"
                //"Accept-Encoding: gzip, deflate\r\n"
                //"Connection: keep-alive\r\n\r\n";

void simpleHttpServer::parseRequest(std::string buffer) {
    const char* buf = buffer.c_str();
    const char* msg_end = "\r";

    const char* head = buf;
    const char* tail = buf;

    // Find request type
    while( tail != msg_end && *tail != ' ' ) ++tail;
    http_request["Type"] = std::string(head, tail);

    head = tail;
    // Find path
    while( tail != msg_end && *tail != ' ' ) ++tail;
    http_request["Path"] = std::string(head, tail);

    // Find HTTP version
    while( tail != msg_end && *tail == ' ' ) ++tail;
	head = tail;
	while( tail != msg_end && *tail != '\r' ) ++tail;
	http_request["Version"] = std::string( head, tail );
    if( tail != msg_end) ++tail; // skip '\r'
	

	 // Map all headers from a key to a value
	//head = tail;
	//while (head != msg_end && *head != '\r') {
		//while (tail != msg_end && *tail != '\r') ++tail;
		////const char *colon = memchr(head, tail, ':');
        //std::string tmp(head, tail);
        //const char* colon = (const char*)tmp.find(':');
		//if (colon == NULL) {
			//// TODO: malformed headers, what should happen?
			//break;
		//}
		//const char *value = colon+1;
		//while (value != tail && *value == ' ') ++value;
		//http_request[ std::string(head, colon) ] = std::string(value, tail);
		//head = tail+1;
		//// TODO: what about the trailing '\n'?
	//}
}
