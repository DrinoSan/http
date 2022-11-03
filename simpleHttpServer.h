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
#include <thread>

// Project Headers
#include "HttpMessage.h"
#include "HttpParser.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

// Defining of some values
#define BACK_LOG 100

class SimpleHttpServer_t {
private:
  int setNonBlocking(int sockfd);
  void createSocket();

public:
  SimpleHttpServer_t();
  ~SimpleHttpServer_t() = default;

  using HttpRequestHandler_t = std::function<HttpResponse_t(const HttpRequest_t &)>;

  void registerRequestHandler(std::string uri, HttpRequest_t::HttpMethode methode,
                              HttpRequestHandler_t callback);

  bool startServer(std::string ipAddr = "", int64_t port = 8000);

private:
  HttpParser_t httpParser;
  std::map<std::string, std::string> http_request;
  std::map<std::string,
           std::map<HttpRequest_t::HttpMethode, HttpRequestHandler_t>>
      requestHandler;

  int sockfd;
  std::thread listenerThread;

  // Handler for not registerd paths
  HttpResponse_t pageNotFound(HttpRequest_t* httpReq);
};

#endif // SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H
