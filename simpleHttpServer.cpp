//
// Created by becirbeg on 22.10.2022.
//

#include "SimpleHttpServer.h"
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <cstdlib>
#include <cstring>
#include <thread>
#include <utility>
#include "HttpRequest.h"
#include "HttpResponse.h"

// get sockaddr, IPv4 or IPv6:
// This Function is important for the inet_ntop function later on if we use
// sockaddr_storage
//----------------------------------------------------------------------------
void* get_in_addr(struct sockaddr* sa) {
   if (sa->sa_family == AF_INET) {
      return &(((struct sockaddr_in*)sa)->sin_addr);
   }

   return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//----------------------------------------------------------------------------
SimpleHttpServer_t::SimpleHttpServer_t() {
   createSocket();
   kq = kqueue();
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::registerRequestHandler(
    std::string uri, HttpRequest_t::HttpMethode methode,
    HttpRequestHandler_t callback) {

   requestHandler[uri].insert(std::make_pair(methode, callback));
}

//----------------------------------------------------------------------------
int SimpleHttpServer_t::setNonBlocking(int sockfd) {
   int flags, s;
   flags = fcntl(sockfd, F_GETFL, 0);
   if (flags == -1) {
      perror("fcntl");
      return -1;
   }
   flags |= O_NONBLOCK;
   s = fcntl(sockfd, F_SETFL, flags);
   if (s == -1) {
      perror("fcntl");
      return -1;
   }
   return 0;
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::listen_and_accept() {
   // Setting basic variables needed for accepting new connections from
   // clients
   int32_t client_len, socket_connection_fd;
   struct sockaddr_in client_addr;

   int worker_idx{0};

   while (true) {
      sockInfos_t* newSocketConnection = new sockInfos_t;
      newSocketConnection->sockfd =
          accept(listeningSocket.sockfd, (struct sockaddr*)&client_addr,
                 (socklen_t*)&client_len);
      if (newSocketConnection->sockfd == -1) {
         std::cout << "Accept socket failed" << std::endl;
         std::cout << "SockFD: " << newSocketConnection->sockfd << std::endl;
         delete newSocketConnection;
         continue;
      }

      // Put this new socket connection also as a 'filter' event
      // to watch in kqueue, so we can now watch for events on this
      // new socket.
      EV_SET(working_chevents[worker_idx], newSocketConnection->sockfd,
             EVFILT_READ, EV_ADD, 0, 0, newSocketConnection);
      if (kevent(working_kqueue_fd[worker_idx], working_chevents[worker_idx], 1,
                 NULL, 0, NULL) < 0) {
         std::cout << "kevent error 3: " << errno << std::endl;
         exit(EXIT_FAILURE);
      }

      ++worker_idx;

      if (worker_idx == NUM_WORKERS)
         worker_idx = 0;
   }
   close(listeningSocket.sockfd);
}

//----------------------------------------------------------------------------
HttpRequest_t SimpleHttpServer_t::handle_read(
    SimpleHttpServer_t::sockInfos_t* sockInfo) {

   HttpRequest_t httpRequest;
   memset(httpRequest.buffer, '\0', BUFFER_SIZE);

   auto bytesRead = recv(sockInfo->sockfd, httpRequest.buffer, BUFFER_SIZE, 0);
   if (bytesRead < 5)
      return httpRequest;

   httpParser.parseRequest(&httpRequest, httpRequest.headers);

   return httpRequest;
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::handle_write(SimpleHttpServer_t::sockInfos_t* sockInfo,
                                      HttpRequest_t httpRequest) {

   auto it_uri = requestHandler.find(httpRequest.httpUri);
   if (it_uri == requestHandler.end()) {
      HttpResponse_t httpResponse = pageNotFound(&httpRequest);
      send(sockInfo->sockfd, httpResponse.httpMessage,
           httpResponse.httpMessageLength, 0);
      return;
   }
   auto it_methode = it_uri->second.find(httpRequest.httpMethode);
   if (it_methode == it_uri->second.end()) {
      std::cout << "Methode not registed for this path: " << httpRequest.httpUri
                << std::endl;
      return;
   }
   auto resp = it_methode->second;
   auto response = resp(httpRequest);
   send(sockInfo->sockfd, response.httpMessage, response.httpMessageLength, 0);
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::prepare_kqueue_events() {

   for (int i = 0; i < NUM_WORKERS; ++i) {
      if ((working_kqueue_fd[i] = kqueue()) < 0) {
         std::cout << "Could not create worker fd for kqueue" << std::endl;
         exit(EXIT_FAILURE);
      }
   }
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::process_worker_events(int worker_idx) {

   int new_events;  //, socket_connection_fd, client_len;

   // File descriptor for kqueue
   int worker_kfd = working_kqueue_fd[worker_idx];

   for (;;) {

      new_events =
          kevent(worker_kfd, NULL, 0, working_events[worker_idx], 1, NULL);

      if (new_events == -1) {
         std::cout << "Kevent error 2" << std::endl;
         exit(EXIT_FAILURE);
      }

      for (int i = 0; i < new_events; i++) {
         int event_fd = working_events[worker_idx][i].ident;

         SimpleHttpServer_t::sockInfos_t* sockInfo =
             reinterpret_cast<SimpleHttpServer_t::sockInfos_t*>(
                 working_events[worker_idx][i].udata);
         // When the client disconnects an EOF is sent. By
         // closing the file descriptor the event is
         // automatically removed from the kqueue
         if (working_events[worker_idx][i].flags & EV_EOF) {
            std::cout << "Client has disconnected" << std::endl;
            close(event_fd);
            close(sockInfo->sockfd);
            delete sockInfo;
         }
         // If the new event's file descriptor is the same as the
         // listening socket's file descriptor, we are sure that
         // a new client wants to connect to our socket.
         else if (event_fd == listeningSocket.sockfd) {
            std::cout << "THIS CAN NOT HAPPEN" << std::endl;
            continue;

         } else if (working_events[worker_idx][i].filter & EVFILT_READ) {
            // Read bytes from socket
            // Converting

            HttpRequest_t httpRequest = handle_read(sockInfo);
            handle_write(sockInfo, std::move(httpRequest));
         }
      }
   }
}

//----------------------------------------------------------------------------
bool SimpleHttpServer_t::startServer(std::string ipAddr, int64_t port) {

   sockaddr_in sockaddr;
   sockaddr.sin_family = AF_INET;
   if (ipAddr != "") {
      inet_pton(AF_INET, ipAddr.c_str(), &(sockaddr.sin_addr));
   } else {
      sockaddr.sin_addr.s_addr = INADDR_ANY;
   }
   sockaddr.sin_port =
       htons(port);  // Hton converts number to network byte order

   // Setting socket to non blocking
   //setNonBlocking(listeningSocket.sockfd);

   if (bind(listeningSocket.sockfd, (struct sockaddr*)&sockaddr,
            sizeof(sockaddr)) < 0) {
      std::cout << "Failed to bind to port " << port << ". errno: " << errno
                << std::endl;
      return false;
   }

   // Start listening. Hold at most BACKL_LOG connection in the queue
   if (listen(listeningSocket.sockfd, BACK_LOG) < 0) {
      std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
      exit(EXIT_FAILURE);
   }

   // Here we need to setup the kqueue for each worker thread
   prepare_kqueue_events();

   // start listnener thread here for incoming connections
   listenerThread = std::thread(&SimpleHttpServer_t::listen_and_accept, this);
   // We also need to call join afterwards probably in a stopServer methode

   std::cout << "Server waiting for connections...\n";

   for (int i = 0; i < NUM_WORKERS; ++i) {
      workerThread[i] =
          std::thread(&SimpleHttpServer_t::process_worker_events, this, i);
   }

   listenerThread.join();
   for (int i = 0; i < NUM_WORKERS; ++i) {
      workerThread[i].join();
   }

   std::cout << "WE ARE FINISHED HERE AND ARE CLOSING THE PROGRAM" << std::endl;

   return true;
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::createSocket() {
   // Create a socket (IPv4, TCP)
   listeningSocket.sockfd = socket(AF_INET, SOCK_STREAM, 0);

   if (listeningSocket.sockfd == -1) {
      std::cout << "Failed to create socket. errno: " << errno << std::endl;
      exit(EXIT_FAILURE);
   }
}

//----------------------------------------------------------------------------
HttpResponse_t SimpleHttpServer_t::pageNotFound(HttpRequest_t* httpReq) {
   HttpResponse_t httpResponse{HttpResponse_t::HttpStatusCode::NotFound};

   // Setting some headers
   httpResponse.setHeader("Server", "Sandi");
   httpResponse.setHeader("Content-Type", "text/html");

   // Preparing answer from server
   std::string resp_msg = "<h1>404 Page Not found</h1>";

   // Building Body
   httpResponse.buildResponseBody(resp_msg);

   return httpResponse;
}
