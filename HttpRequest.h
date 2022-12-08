//
// Created by becirbeg on 28.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPREQUEST_H
#define SIMPLEHTTPSERVER2_HTTPREQUEST_H

// System Headers
#include <string>

// Project Headers
#include "HttpMessage.h"

#define BUFFER_SIZE 20000

class HttpRequest_t : public HttpMessage {
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
   HttpRequest_t(char* msg);
   HttpRequest_t() = default;
   ~HttpRequest_t() = default;
   char* parseMethodePathVersion(char* msg);
   std::string httpMethodeToString(HttpMethode method);
   HttpMethode stringToHttpMethode(std::string methode);

  public:
   HttpMethode httpMethode;
   std::string httpUri;
   std::string resource;
   char buffer[BUFFER_SIZE];
};

#endif  // SIMPLEHTTPSERVER2_HTTPREQUEST_H
