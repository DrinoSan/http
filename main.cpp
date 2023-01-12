#include <random>

#include "HttpRequest_t.h"
#include "HttpResponse_t.h"
#include "SimpleHttpServer.h"
#include "JsonParser_t.h"
#include "Handler.h"

using namespace rapidjson;

int main()
{
	SimpleHttpServer_t server;

	// Registering path: /sand for GET with helloWorld callback
	server.registerRequestHandler("/sand", HttpRequest_t::HttpMethode::GET,
			helloWorld);

	server.registerRequestHandler("/helloworld", HttpRequest_t::HttpMethode::GET,
			helloWorldHandler);

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

	server.startServer("127.0.0.1", 443);
	return 0;
}
