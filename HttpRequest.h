//
// Created by becirbeg on 28.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPREQUEST_H
#define SIMPLEHTTPSERVER2_HTTPREQUEST_H


#include "HttpMessage.h"
#include <string>

class HttpRequest : public HttpMessage {
    
    public :
        std::string httpMethode;
        std::string httpUri;
};


#endif  //SIMPLEHTTPSERVER2_HTTPREQUEST_H
