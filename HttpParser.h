//
// Created by becirbeg on 28.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPPARSER_H
#define SIMPLEHTTPSERVER2_HTTPPARSER_H

// System headers
#include <string_view>

// Project Headers
#include "HttpMessage.h"
#include "HttpRequest.h"

class HttpParser_t
{
private:
//    char* parseMethodePathVersion(HttpRequest_t* httpReq);

public:
    void mapHeaders(HttpRequest_t& httpReq);
//    bool parseRequest(HttpRequest_t* httpReq,
//                      std::map<std::string, std::string> &headers);
};

#endif  // SIMPLEHTTPSERVER2_HTTPPARSER_H
