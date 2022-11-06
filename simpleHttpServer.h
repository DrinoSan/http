//
// Created by becirbeg on 22.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H
#define SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H

// System Headers
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <thread>

// Project Headers
#include "HttpMessage.h"
#include "HttpParser.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

// Defining of some values
constexpr int BACK_LOG = 100000;
constexpr int NUM_WORKERS = 5;
constexpr int NUM_EVENTS = 10000;

class SimpleHttpServer_t {
  private:
   int setNonBlocking(int sockfd);
   void createSocket();
   void listen_and_accept();
   void prepare_kqueue_events();
   void process_worker_events(int worker_idx);

  public:
   SimpleHttpServer_t();
   ~SimpleHttpServer_t() = default;

   using HttpRequestHandler_t =
       std::function<HttpResponse_t(const HttpRequest_t&)>;

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
      HttpRequest_t httpReq;
      HttpResponse_t httpResp;
      std::function<void(struct sockInfos_t* sockInfo)> read_handler;
      std::function<void(struct sockInfos_t* sockinfo)> write_handler;
   };

   HttpRequest_t handle_read(struct sockInfos_t* sockInfo);
   void handle_write(struct sockInfos_t* sockInfo, HttpRequest_t httpRequest);
   sockInfos_t listeningSocket;

   struct kevent change_event[NUM_EVENTS];
   struct kevent event[NUM_EVENTS];
   int working_kqueue_fd[NUM_WORKERS];
   struct kevent working_events[NUM_WORKERS][NUM_EVENTS];
   struct kevent working_chevents[NUM_WORKERS][NUM_EVENTS];

   std::thread listenerThread;
   std::thread workerThread[NUM_WORKERS];
   int kq;

   // Handler for not registerd paths
   HttpResponse_t pageNotFound(HttpRequest_t* httpReq);
};

#endif  // SIMPLEHTTPSERVER2_SIMPLEHTTPSERVER_H
