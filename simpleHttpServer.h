//
// Created by becirbeg on 22.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H
#define SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H

// System Headers
#include <arpa/inet.h>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

// Project Headers
#include "HttpMessage.h"
#include "HttpParser.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

// Defining of some values
#define BACK_LOG 100

class simpleHttpServer {
private:
  int setNonBlocking(int sockfd);
public:
  simpleHttpServer() { std::cout << "Starting Server" << std::endl; }
  ~simpleHttpServer() = default;

  using HttpRequestHandler_t = std::function<HttpResponse(const HttpRequest &)>;

  void registerRequestHandler(std::string uri, HttpRequest::HttpMethode methode,
                              HttpRequestHandler_t callback);

  bool startServer(std::string ipAddr = "", int64_t port = 8000);

private:
  HttpParser httpParser;
  std::map<std::string, std::string> http_request;
  std::map<std::string,
           std::map<HttpRequest::HttpMethode, HttpRequestHandler_t>>
      requestHandler;
};

#endif // SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H
