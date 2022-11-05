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
#include <sys/event.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>

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
  void listen_and_accept();

public:
  SimpleHttpServer_t();
  ~SimpleHttpServer_t() = default;

  using HttpRequestHandler_t =
      std::function<HttpResponse_t(const HttpRequest_t &)>;

  void registerRequestHandler(std::string uri,
                              HttpRequest_t::HttpMethode methode,
                              HttpRequestHandler_t callback);

  bool startServer(std::string ipAddr = "", int64_t port = 8000);

private:
  HttpParser_t httpParser;
  std::map<std::string, std::string> http_request;
  std::map<std::string,
           std::map<HttpRequest_t::HttpMethode, HttpRequestHandler_t>>
      requestHandler;

  struct sockInfos_t {
    int sockfd;
    uintptr_t ptrAddress;
    std::function<void(struct sockInfos_t *sockInfo)> read_handler;
    std::function<void(struct sockInfos_t *sockinfo)> write_handler;
  };

  HttpRequest_t handle_read(struct sockInfos_t *sockInfo);
  void handle_write(struct sockInfos_t *sockInfo, HttpRequest_t httpRequest);
  sockInfos_t listeningSocket;
  struct kevent change_event[40];
  struct kevent event[40];
  std::thread listenerThread;
  int kq;

  // Handler for not registerd paths
  HttpResponse_t pageNotFound(HttpRequest_t *httpReq);
};

#endif // SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H
