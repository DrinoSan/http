//
// Created by becirbeg on 28.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPRESPONSE_H
#define SIMPLEHTTPSERVER2_HTTPRESPONSE_H


#include "HttpMessage.h"
#include <string>

class HttpResponse : public HttpMessage {
    
    public :
        std::string statusCode;
};


#endif  //SIMPLEHTTPSERVER2_HTTPRESPONSE_H
