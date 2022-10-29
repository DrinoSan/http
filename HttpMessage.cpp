//
// Created by becirbeg on 22.10.2022.
//

// System Headers
#include <iostream>

// Project Headers
#include "HttpMessage.h"
#include "HttpParser.h"

HttpMessage::HttpMessage(char* msg) : httpRequestBlob{msg} {
    std::cout << "Constructing HttpMessage object!" << std::endl;
}


