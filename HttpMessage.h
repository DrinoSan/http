//
// Created by becirbeg on 22.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPMESSAGE_H
#define SIMPLEHTTPSERVER2_HTTPMESSAGE_H

#include <map>
#include <string>
#include <string_view>

#define NUM_HTTP_HEADERS 50

class HttpMessage {
public:
  //using Headers_t = std::map<std::string, std::string>;
  //bool parseHeaders(Headers_t* headers);
  bool parseHeaders(char* httpMessageBlog);

public:
  std::string httpVersion;
  //Headers_t httpHeaders;
  std::string httpBody;
  char* httpRequestBlob;

  struct headers{
      std::string_view key;
      std::string_view value;
  } headers[NUM_HTTP_HEADERS];

};

#endif // SIMPLEHTTPSERVER2_HTTPMESSAGE_H
