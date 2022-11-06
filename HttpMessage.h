//
// Created by becirbeg on 22.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPMESSAGE_H
#define SIMPLEHTTPSERVER2_HTTPMESSAGE_H

#define NUM_HTTP_HEADERS 50

// System Header
#include <map>
#include <string>
#include <string_view>

// Project Header

class HttpMessage {
  public:
   HttpMessage(char* msg);
   HttpMessage() = default;
   std::map<std::string, std::string> headers;

  private:
  public:
   std::string httpVersion{"HTTP/1.1"};
   std::string httpBody;

   char* httpMessage;
   size_t httpMessageLength;
   std::string httpResponseBody;
};

#endif  // SIMPLEHTTPSERVER2_HTTPMESSAGE_H
