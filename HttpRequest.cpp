//
// Created by becirbeg on 22.10.2022.
//

#include <iostream>

#include "HttpRequest.h"
#include "HttpParser.h"

HttpRequest::HttpRequest(char *msg) : HttpMessage{msg} {
    std::cout << "Constructing HttpRequest object!" << std::endl;
}


