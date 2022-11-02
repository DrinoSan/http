//
// Created by becirbeg on 28.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPREQUEST_H
#define SIMPLEHTTPSERVER2_HTTPREQUEST_H

// System Headers
#include <string>

// Project Headers
#include "HttpMessage.h"

class HttpRequest : public HttpMessage {
public:
  enum class HttpMethode {
    GET,
    POST,
    DELETE,
    PUT,
    UPDATE,
    UNKNOWN,
  };

public:
  HttpRequest(char *msg);
  HttpRequest() = default;
  ~HttpRequest() = default;
  char *parseMethodePathVersion(char *msg);
  std::string httpMethodeToString(HttpMethode method);
  HttpMethode stringToHttpMethode(std::string methode);

public:
  HttpMethode httpMethode;
  std::string httpUri;
};

#endif // SIMPLEHTTPSERVER2_HTTPREQUEST_H