//
// Created by becirbeg on 22.10.2022.
//

#include "SimpleHttpServer.h"
#include "HttpRequest.h"
#include <cstring>
#include <netinet/in.h>
#include <sys/socket.h>
#include <utility>

// get sockaddr, IPv4 or IPv6:
// This Function is important for the inet_ntop function later on if we use
// sockaddr_storage
//----------------------------------------------------------------------------
void *get_in_addr(struct sockaddr *sa) {
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in *)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

//----------------------------------------------------------------------------
void simpleHttpServer::registerRequestHandler(std::string uri,
                                              HttpRequest::HttpMethode methode,
                                              HttpRequestHandler_t callback) {

  requestHandler[uri].insert(std::make_pair(methode, callback));
}

//----------------------------------------------------------------------------
bool simpleHttpServer::startServer(char buffer[], std::string ipAddr,
                                   int64_t port) {
  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1) {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Listen to port 9999 on any address
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  if (ipAddr != "") {
    inet_pton(AF_INET, ipAddr.c_str(), &(sockaddr.sin_addr));
  } else {
    sockaddr.sin_addr.s_addr = INADDR_ANY;
  }
  sockaddr.sin_port = htons(port); // Hton converts number to network byte order

  if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
    std::cout << "Failed to bind to port " << port << ". errno: " << errno
              << std::endl;
    return false;
  }

  std::cout << "Socket FileDeskriptor: " << sockfd << " " << std::endl;

  // Start listening. Hold at most 10 connection in the queue
  if (listen(sockfd, BACK_LOG) < 0) {
    std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Grab connection from the queue
  auto addrlen = sizeof(sockaddr);
  // struct sockaddr_storage their_addr; // connector's address information
  struct sockaddr_in their_addr;

  int newConnectionFD;
  char s[INET_ADDRSTRLEN];

  while ((newConnectionFD = accept(sockfd, (struct sockaddr *)&sockaddr,
                                   (socklen_t *)&their_addr)) > 0) {
    if (newConnectionFD < 0) {
      std::cout << "Failed to grab connection. errno: " << errno << std::endl;
      exit(EXIT_FAILURE);
    }

    // Converting
    inet_ntop(AF_INET, &(their_addr.sin_addr), s, INET_ADDRSTRLEN);
    std::cout << "Got Connection from: " << s << std::endl;

    if (fork() == 0) {
      close(sockfd);

      char buffer[300];
      auto bytesRead = recv(newConnectionFD, buffer, 300, 0);
      HttpRequest httpRequest{buffer};
      httpParser.parseRequest(&httpRequest, httpRequest.headers);

      auto it_uri = requestHandler.find(httpRequest.httpUri);
      if (it_uri == requestHandler.end()) {
        std::cout << "Path not registerd" << std::endl;
        return false;
      }
      auto it_methode = it_uri->second.find(httpRequest.httpMethode);
      if (it_methode == it_uri->second.end()) {
        std::cout << "Methode not registed for this path: "
                  << httpRequest.httpUri << std::endl;
        return false;
      }
      auto resp = it_methode->second;
      auto response = resp(httpRequest);
      send(newConnectionFD, response.httpMessage, response.httpMessageLength,
           0);

      close(newConnectionFD);
      exit(0);
    }
    close(newConnectionFD);
  }

  close(sockfd);
  return true;
}
