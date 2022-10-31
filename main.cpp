#include <cstdlib>
#include <iostream>
#include <string>

#include "HttpMessage.h"
#include "HttpParser.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "simpleHttpServer.h"

int main() {
  // std::cout << "Hello, World!" << std::endl;

  simpleHttpServer server;

  char buffer[500];

  // char msg[] =
  //"GET / HTTP/1.1\r\n"
  //"Host: 192.241.213.46:6880\r\n"
  //"Upgrade-Insecure-Requests: 1\r\n"
  //"Accept: "
  //"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
  //"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) "
  //"AppleWebKit/602.4.8 (KHTML, like Gecko) Version/10.0.3 "
  //"Safari/602.4.8\r\n"
  //"Accept-Language: en-us\r\n"
  //"Accept-Encoding: gzip, deflate\r\n"
  //"Connection: keep-alive\r\n\r\n";

  auto helloWorld = [](const HttpRequest &request) -> HttpResponse {
    std::string resp = "HTTP/1.1 200 OK\r\n"
                       "Server: Hello\r\n"
                       "Content-Length: 22\r\n"
                       "Content-Type: text/plain\r\n"
                       "\r\n"
                       "Hello, world!!!tester!";
    HttpResponse httpResponse{HttpResponse::HttpStatusCode::Ok};
    httpResponse.httpRequestBlob_2 = resp;
    return httpResponse;
  };

  HttpRequest httpRequest;
  // HttpParser httpParser;
  // httpParser.parseRequest(&httpRequest, httpRequest.headers);
  server.registerRequestHandler("/sand", HttpRequest::HttpMethode::GET,
                                helloWorld);

  // std::cout << "-.-.-.-.-.-.-.-.-.-.-.-" << std::endl;
  // std::cout << "VERSION: " << httpRequest.httpVersion << std::endl;
  // std::cout << "URI    : " << httpRequest.httpUri << std::endl;
  // std::cout << "Methode: " <<
  // httpRequest.httpMethodeToString(httpRequest.httpMethode) << std::endl;

  server.startServer(buffer, "0.0.0.0", 8000);
  return 0;
}
