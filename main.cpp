#include <cstdlib>
#include <iostream>
#include <string>

#include "HttpMessage.h"
#include "HttpParser.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "SimpleHttpServer.h"

int main() {

  SimpleHttpServer_t server;

  // Creating callback function
  auto helloWorld = [](const HttpRequest_t &request) -> HttpResponse_t {
    // Creating Response Object with StatusCode OK -> 200
    HttpResponse_t httpResponse{HttpResponse_t::HttpStatusCode::Ok};

    // Setting some headers
    httpResponse.setHeader("Server", "Sandi");
    httpResponse.setHeader("Content-Type", "text/plain");

    // Preparing answer from server
    std::string resp_msg = "Hello World from Sandi's Server";

    // Building Body
    httpResponse.buildResponseBody(resp_msg);

    return httpResponse;
  };

  // Creating callback function
  auto home = [](const HttpRequest_t &request) -> HttpResponse_t {
    // Creating Response Object with StatusCode OK -> 200
    HttpResponse_t httpResponse{HttpResponse_t::HttpStatusCode::Ok};

    // Setting some headers
    httpResponse.setHeader("Server", "Sandi");
    httpResponse.setHeader("Content-Type", "text/html");

    // Preparing answer from server
    std::string resp_msg = "<h1>Home screen</h1>";

    // Building Body
    httpResponse.buildResponseBody(resp_msg);

    return httpResponse;
  };

  // Creating callback function
  auto dummy = [](const HttpRequest_t &request) -> HttpResponse_t {
    // Creating Response Object with StatusCode OK -> 200
    HttpResponse_t httpResponse{HttpResponse_t::HttpStatusCode::Ok};

    // Setting some headers
    httpResponse.setHeader("Server", "Sandi");
    httpResponse.setHeader("Content-Type", "text/html");

    // Preparing answer from server
    std::string resp_msg = "<h1>DUMMY</h1>";

    // Building Body
    httpResponse.buildResponseBody(resp_msg);

    return httpResponse;
  };

  // Registering path: /sand for GET with helloWorld callback
  server.registerRequestHandler("/sand", HttpRequest_t::HttpMethode::GET,
                                helloWorld);

  // Registering path: / for GET with home callback
  server.registerRequestHandler("/", HttpRequest_t::HttpMethode::GET, home);

  server.registerRequestHandler("/dummy", HttpRequest_t::HttpMethode::GET, dummy);

  server.startServer("127.0.0.1", 8000);
  return 0;
}
