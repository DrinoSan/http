#include <cstdlib>
#include <iostream>
#include <string>

#include "HttpMessage.h"
#include "HttpParser.h"
#include "HttpRequest.h"
//#include "simpleHttpServer.h"

int main() {
  // std::cout << "Hello, World!" << std::endl;

  // simpleHttpServer server;

  // if( ! server.startServer( "0.0.0.0", 9000 ) ) {
  // std::cout << "PORT: 8000" << std::endl;
  // server.startServer("0.0.0.0", 8000);
  //}
  char msg[] =
      "GET / HTTP/1.1\r\n"
      "Host: 192.241.213.46:6880\r\n"
      "Upgrade-Insecure-Requests: 1\r\n"
      "Accept: "
      "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
      "User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) "
      "AppleWebKit/602.4.8 (KHTML, like Gecko) Version/10.0.3 "
      "Safari/602.4.8\r\n"
      "Accept-Language: en-us\r\n"
      "Accept-Encoding: gzip, deflate\r\n"
      "Connection: keep-alive\r\n\r\n";

  HttpRequest httpRequest{msg};
  HttpParser httpParser;
  httpParser.parseRequest(&httpRequest, httpRequest.headers);

  std::cout << "-.-.-.-.-.-.-.-.-.-.-.-" << std::endl;
  std::cout << "VERSION: " << httpRequest.httpVersion << std::endl;
  std::cout << "URI    : " << httpRequest.httpUri << std::endl;
  std::cout << "Methode: " << httpRequest.httpMethode << std::endl;

  return 0;
}
