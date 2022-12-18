//
// Created by becirbeg on 22.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPMESSAGE_T_H
#define SIMPLEHTTPSERVER2_HTTPMESSAGE_T_H

#define NUM_HTTP_HEADERS 50

// System Header
#include <map>
#include <string>
#include <string_view>

// Project Header

class HttpMessage_t
{
public:
	HttpMessage_t(char* msg);

	HttpMessage_t() = default;

	std::map<std::string, std::string> headers;

private:
public:
	std::string httpVersion{ "HTTP/1.1" };
	std::string httpBody;

	std::string httpMessage;
	size_t httpMessageLength;
	std::string httpResponseBody;
};

#endif  // SIMPLEHTTPSERVER2_HTTPMESSAGE_T_H
