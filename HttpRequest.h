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
    HttpRequest(char* msg);
    char* parseMethodePathVersion(char* msg);

public:
  std::string httpMethode;
  std::string httpUri;
};

#endif // SIMPLEHTTPSERVER2_HTTPREQUEST_H
