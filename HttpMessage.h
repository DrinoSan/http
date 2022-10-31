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
  HttpMessage(char *msg);
  HttpMessage() = default;
  //struct HEADERS {
    //std::string_view key;
    //std::string_view value;
  //} headers[NUM_HTTP_HEADERS];
  std::map<std::string, std::string> headers;

private:
public:
  std::string httpVersion;
  std::string httpBody;
  char *httpRequestBlob;
  std::string httpRequestBlob_2;

};

#endif // SIMPLEHTTPSERVER2_HTTPMESSAGE_H
