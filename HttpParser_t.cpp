// System Headers
#include <cstddef>
#include <iostream>
#include <string_view>
#include <utility>
#include <sstream>

// Project Headers
#include "HttpParser_t.h"
#include "HttpRequest_t.h"

//----------------------------------------------------------------------------
void HttpParser_t::mapHeaders(HttpRequest_t& httpReq)
{
	// First line is METHODE, PATH, VERSION aka Request line
	// Remaining lines are the headers
	std::istringstream stream(httpReq.buffer);
	std::string tester = httpReq.buffer;
	std::string method, path, version;

	// Parsing methode, path and version
	stream >> method >> path >> version;

	// Converting methode to enum
	httpReq.httpMethode = httpReq.stringToHttpMethode(method);

	httpReq.httpUri = path;
	httpReq.httpVersion = version;

	auto requestMessage = stream.str();

	// Declaring variables for positions of key and values
	std::string::size_type key_pos = 0;
	std::string::size_type key_end;
	std::string::size_type val_pos;
	std::string::size_type val_end;

	auto end_of_headers = tester.find("\r\n\r\n");
	// Finding end of httpMessage
	if (end_of_headers == std::string::npos)
	{
		// Todo: What shall we do here?
		return;
	}

	while ((key_end = requestMessage.find(':', key_pos)) != std::string::npos && key_end < end_of_headers)
	{
		if ((val_pos = requestMessage.find_first_not_of(": ", key_end)) == std::string::npos)
		{
			break;
		}

		val_end = requestMessage.find("\n", val_pos);
		httpReq.headers.emplace(requestMessage.substr(key_pos, key_end - key_pos),
				requestMessage.substr(val_pos, val_end - val_pos - 1));

		key_pos = val_end;
		if (key_pos != std::string::npos)
		{
			++key_pos;
		}
	}

	auto it = httpReq.headers.find("Content-Length");
	if (it == httpReq.headers.end())
	{
		return;
	}

	// Adding 4 to the position of end_of_headers because \r\n\r\n
	end_of_headers += 4;

	// Let's trust, that the content_length is correct
	// Constructing std::string from a char array.
	// We start at the 0th position and add end_of_headers <--- this is our START
	// We get all chars till start + content_length        <--- this is our END
	//                  //_____________START_______________,_________________________________END_______________________________________
	std::string bodyMessage(httpReq.buffer + end_of_headers,
			httpReq.buffer + end_of_headers + (size_t)+std::stoi(it->second));

	httpReq.httpBody = bodyMessage;
}