//
// Created by becirbeg on 22.10.2022.
//

#include "SimpleHttpServer.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/event.h>
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
SimpleHttpServer_t::SimpleHttpServer_t() {
  createSocket();
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
bool SimpleHttpServer_t::startServer(std::string ipAddr, int64_t port) {

  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  if (ipAddr != "") {
    inet_pton(AF_INET, ipAddr.c_str(), &(sockaddr.sin_addr));
  } else {
    sockaddr.sin_addr.s_addr = INADDR_ANY;
  }
  sockaddr.sin_port = htons(port); // Hton converts number to network byte order

  // Setting socket to non blocking
  setNonBlocking(sockfd);

  if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) {
    std::cout << "Failed to bind to port " << port << ". errno: " << errno
              << std::endl;
    return false;
  }

  // Start listening. Hold at most 10 connection in the queue
  if (listen(sockfd, BACK_LOG) < 0) {
    std::cout << "Failed to listen on socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  std::cout << "Server waiting for connections...\n";

  int kq, new_events, socket_connection_fd, client_len;
  struct kevent change_event[40];
  struct kevent event[40];
  struct sockaddr_in serv_addr, client_addr;
  client_len = sizeof(client_addr);
  kq = kqueue();

  EV_SET(change_event, sockfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, 0);
  if (kevent(kq, change_event, 1, NULL, 0, NULL) == -1) {
    std::cout << "Kevent error" << std::endl;
    exit(EXIT_FAILURE);
  }

  for (;;) {
    new_events = kevent(kq, NULL, 0, event, 1, NULL);
    if (new_events == -1) {
      std::cout << "Kevent error 2" << std::endl;
      exit(EXIT_FAILURE);
    }

    for (int i = 0; i < new_events; i++) {
      int event_fd = event[i].ident;

      // When the client disconnects an EOF is sent. By closing the file
      // descriptor the event is automatically removed from the kqueue
      if (event[i].flags & EV_EOF) {
        std::cout << "Client has disconnected" << std::endl;
        close(event_fd);
      }
      // If the new event's file descriptor is the same as the listening
      // socket's file descriptor, we are sure that a new client wants
      // to connect to our socket.
      else if (event_fd == sockfd) {
        // Incoming socket connection on the listening socket.
        // Create a new socket for the actual connection to client.
        socket_connection_fd = accept(event_fd, (struct sockaddr *)&client_addr,
                                      (socklen_t *)&client_len);
        if (socket_connection_fd == -1) {
          std::cout << "Accept socket failed" << std::endl;
        }

        // Put this new socket connection also as a 'filter' event
        // to watch in kqueue, so we can now watch for events on this
        // new socket.
        EV_SET(change_event, socket_connection_fd, EVFILT_READ, EV_ADD, 0, 0,
               NULL);
        if (kevent(kq, change_event, 1, NULL, 0, NULL) < 0) {
          std::cout << "kevent error 3" << std::endl;
        }
      } else if (event[i].filter & EVFILT_READ) {
        // Read bytes from socket
        // Converting
        char s[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), s, INET_ADDRSTRLEN);
        std::cout << "Got Connection from: " << s << std::endl;

        HttpRequest_t httpRequest;
        memset(httpRequest.buffer, '\0', BUFFER_SIZE);
        auto bytesRead =
            recv(socket_connection_fd, httpRequest.buffer, BUFFER_SIZE, 0);
        std::cout << "SAND RECV: " << httpRequest.buffer << std::endl;
        std::cout << "----------------------------------" << std::endl;
        if (bytesRead > 5)
          httpParser.parseRequest(&httpRequest, httpRequest.headers);
        else
          continue;

        auto it_uri = requestHandler.find(httpRequest.httpUri);
        if (it_uri == requestHandler.end()) {
          std::cout << "Path not registerd" << std::endl;
          HttpResponse_t httpResponse = pageNotFound(&httpRequest);
          send(socket_connection_fd, httpResponse.httpMessage,
               httpResponse.httpMessageLength, 0);
          close(event_fd);
          close(socket_connection_fd);
          continue;
        }
        auto it_methode = it_uri->second.find(httpRequest.httpMethode);
        if (it_methode == it_uri->second.end()) {
          std::cout << "Methode not registed for this path: "
                    << httpRequest.httpUri << std::endl;
          return false;
        }
        auto resp = it_methode->second;
        auto response = resp(httpRequest);
        send(socket_connection_fd, response.httpMessage,
             response.httpMessageLength, 0);
      }
    }
  }

  close(sockfd);
  return true;
}

//----------------------------------------------------------------------------
void SimpleHttpServer_t::createSocket() {
  // Create a socket (IPv4, TCP)
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
}

//----------------------------------------------------------------------------
HttpResponse_t SimpleHttpServer_t::pageNotFound(HttpRequest_t *httpReq) {
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
