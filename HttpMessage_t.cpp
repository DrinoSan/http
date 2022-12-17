//
// Created by becirbeg on 22.10.2022.
//

// System Headers
#include <iostream>

// Project Headers
#include "HttpMessage_t.h"
#include "HttpParser_t.h"

HttpMessage_t::HttpMessage_t(char* msg) : httpMessage{msg}
{
    httpVersion = "HTTP/1.1";
}
