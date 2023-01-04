//
// Created by becirbeg on 28.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPPARSER_T_H
#define SIMPLEHTTPSERVER2_HTTPPARSER_T_H

// Project Headers
#include "HttpMessage_t.h"
#include "HttpRequest_t.h"

// Todo: Must this be a class?
class HttpParser_t
{
public:
	HttpParser_t() = default;

	~HttpParser_t() = default;

	void mapHeaders(HttpRequest_t& httpReq);
};

#endif  // SIMPLEHTTPSERVER2_HTTPPARSER_T_H
