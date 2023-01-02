#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <random>

#include <chrono>
#include <thread>

#include "HttpRequest_t.h"
#include "HttpResponse_t.h"
#include "SimpleHttpServer.h"
#include "JsonParser_t.h"

using namespace rapidjson;

int main()
{
	SimpleHttpServer_t server;

//	auto serveStatic =
//			[&server](const HttpRequest_t& request) -> HttpResponse_t
//			{
//				HttpResponse_t httpResponse{ HttpResponse_t::HttpStatusCode::Ok };
//
//				// the root_dir path depends on where the binary is executed
//				const auto root_dir = fs::path{ "../static" };
//				server.fileServerPath = "static";
//
//				std::ostringstream stream;
//				size_t fileSize;
//				server.serve_static_file(root_dir, request.resource, stream, fileSize, httpResponse);
//
//				auto body = stream.str();
//
//				httpResponse.httpMessageLength = body.size();
//				httpResponse.httpResponseBody = body;
//				httpResponse.httpMessage = httpResponse.httpResponseBody;
//
//				httpResponse.setHeader("Server", "Sandi");
//				httpResponse.buildResponseBody(body);
//
//				return httpResponse;
//			};

	// Creating callback function
	auto helloWorld = [](const HttpRequest_t& request) -> HttpResponse_t
	{
		// Creating Response Object with StatusCode OK -> 200
		HttpResponse_t httpResponse{ HttpResponse_t::HttpStatusCode::Ok };

		// Setting some headers
		httpResponse.setHeader("Server", "Sandi");
		httpResponse.setHeader("Content-Type", "text/plain");

		// Preparing answer from server
		std::string resp_msg = "Hello World from Sandi's Server";

		// Building Body
		httpResponse.buildResponseBody(resp_msg);

		return httpResponse;
	};

	auto testJsonParsing = [](const HttpRequest_t& request) -> HttpResponse_t
	{
		// Creating Response Object with StatusCode OK -> 200
		HttpResponse_t httpResponse{ HttpResponse_t::HttpStatusCode::Ok };

		// Setting some headers
		httpResponse.setHeader("Server", "Sandi");
		httpResponse.setHeader("Content-Type", "text/plain");

		// Preparing answer from server
		std::string resp_msg = "Hello World from Sandi's Server with Hassan's help";
		for (const auto& [key, value]: request.headers)
		{
			std::cout << "HEADER Key: " << key << " Value: " << value << std::endl;
		}

		if (!request.httpBody.empty())
		{
			std::cout << "Message Body: \n" << request.httpBody << std::endl;

			JsonParser_t jP(request.httpBody);
			std::string type = jP.getTypeOfElement("productId");
			std::cout << jP.dumpJson() << std::endl;
			std::cout << "------------------" << std::endl;

			// This way we need to know already before what the type of the variable is...
			auto& foo = jP.get("productId");
			int val;
			jP.extractValue("productId", val);
			std::string quantity;
			jP.extractValue("quantity", quantity);
			std::vector<int> vec;
			jP.extractValue("arr", vec);
		}
		else
		{
			std::cout << "BODY IS EMPTY" << std::endl;
		}

		// Building Body
		httpResponse.buildResponseBody(resp_msg);

		return httpResponse;
	};

	// Creating callback function
	auto home = [](const HttpRequest_t& request) -> HttpResponse_t
	{
		// Creating Response Object with StatusCode OK -> 200
		HttpResponse_t httpResponse{ HttpResponse_t::HttpStatusCode::Ok };

		// Setting some headers
		httpResponse.setHeader("Server", "Sandi");
		httpResponse.setHeader("Content-Type", "text/html");

		// Preparing answer from server
		std::string resp_msg = "<h1>Home screen</h1>";

		// Building Body
		httpResponse.buildResponseBody(resp_msg);

		return httpResponse;
	};

	// Creating callback function
	auto dummy = [](const HttpRequest_t& request) -> HttpResponse_t
	{
		// Creating Response Object with StatusCode OK -> 200
		HttpResponse_t httpResponse{ HttpResponse_t::HttpStatusCode::Ok };

		// Setting some headers
		httpResponse.setHeader("Server", "Sandi");
		httpResponse.setHeader("Content-Type", "text/html");

		// Preparing answer from server
		std::string resp_msg = "<h1>DUMMY</h1>";

		// Building Body
		httpResponse.buildResponseBody(resp_msg);

		return httpResponse;
	};
	// Creating callback function
	auto getRandomNumber = [](const HttpRequest_t& request) -> HttpResponse_t
	{
		// Creating Response Object with StatusCode OK -> 200
		HttpResponse_t httpResponse{ HttpResponse_t::HttpStatusCode::Ok };

		// Setting some headers
		httpResponse.setHeader("Server", "Sandi");
		httpResponse.setHeader("Content-Type", "text/html");
		httpResponse.setHeader("Access-Control-Allow-Origin", "*");

		std::random_device dev;
		std::mt19937 rng(dev());
		std::uniform_int_distribution<std::mt19937::result_type> dist6(1,100); // distribution in range [1, 6]

		// Preparing answer from server
		std::string resp_msg = std::to_string(dist6(rng));
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

		// Building Body
		httpResponse.buildResponseBody(resp_msg);

		return httpResponse;
	};

	// Registering path: /sand for GET with helloWorld callback
	server.registerRequestHandler("/sand", HttpRequest_t::HttpMethode::GET,
			helloWorld);

	// Registering path: / for GET with home callback
	server.registerRequestHandler("/", HttpRequest_t::HttpMethode::GET, home);

	server.registerRequestHandler("/dummy", HttpRequest_t::HttpMethode::GET,
			dummy);

	server.registerRequestHandler("/dummy/foo/bar", HttpRequest_t::HttpMethode::GET,
			dummy);

	// Register /files/ path as
	server.registerRequestHandler("/files/", HttpRequest_t::HttpMethode::GET,
			server.stripPrefix("/files/", server.fileServer("../static")));

	server.registerRequestHandler("/jsonParse", HttpRequest_t::HttpMethode::POST, testJsonParsing);

	server.registerRequestHandler("/random-number", HttpRequest_t::HttpMethode::GET, getRandomNumber);

	server.registerRequestHandler("/parameter/{:id}", HttpRequest_t::HttpMethode::GET, home);

	server.startServer("127.0.0.1", 8000);
//	server.startServer("192.168.0.32", 8000);
	return 0;
}
