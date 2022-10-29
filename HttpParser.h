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

class HttpParser {
    public :
        bool parseRequest(HttpRequest* httpReq, struct HttpMessage::HEADERS *headers);
        bool parseMethodePathVersion(HttpRequest* httpReq);
};


#endif  //SIMPLEHTTPSERVER2_HTTPPARSER_H
