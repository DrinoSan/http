//
// Created by becirbeg on 28.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPPARSER_H
#define SIMPLEHTTPSERVER2_HTTPPARSER_H


#include "HttpRequest.h"
#include <string>

class HttpParser {
    
    public :
        bool parseHeaders(HttpRequest* httpReq);
};


#endif  //SIMPLEHTTPSERVER2_HTTPPARSER_H
