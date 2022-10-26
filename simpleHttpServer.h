//
// Created by becirbeg on 22.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H
#define SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H

#include <string>
#include <iostream>
#include <map>

#include <cstdlib>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 

class simpleHttpServer {
    
    private :
        bool isParsingFinished( const char* tail );

    public :
        simpleHttpServer() {
            std::cout << "Starting Server" << std::endl; 
        }
        ~simpleHttpServer() = default;


        bool startServer(std::string ipAddr = "", int64_t port=8000);
        void parseRequest(const char*);

    private :
        std::map<std::string, std::string> http_request;

};


#endif //SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H
