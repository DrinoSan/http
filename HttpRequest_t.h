//
// Created by becirbeg on 28.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPREQUEST_T_H
#define SIMPLEHTTPSERVER2_HTTPREQUEST_T_H

// System Headers
#include <string>

// Project Headers
#include "HttpMessage_t.h"

#define BUFFER_SIZE 3000

class HttpRequest_t : public HttpMessage_t
{
public:
	enum class HttpMethode
	{
		GET,
		POST,
		DELETE,
		PUT,
		UPDATE,
		UNKNOWN,
	};

	HttpMethode httpMethode;
	std::string httpUri;
	std::string resource;
	char buffer[BUFFER_SIZE];

public:
	HttpRequest_t(char* msg);

	HttpRequest_t() = default;

	~HttpRequest_t() = default;

	std::string httpMethodeToString(HttpMethode method);

	HttpMethode stringToHttpMethode(std::string methode);

};

#endif  // SIMPLEHTTPSERVER2_HTTPREQUEST_T_H
