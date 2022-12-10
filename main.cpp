#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "HttpMessage.h"
#include "HttpParser.h"
#include "HttpRequest.h"
#include "HttpResponse.h"
#include "SimpleHttpServer.h"

int main()
{

    SimpleHttpServer_t server;

    auto serveStatic =
            [&server](const HttpRequest_t &request) -> HttpResponse_t
            {
                HttpResponse_t httpResponse{HttpResponse_t::HttpStatusCode::Ok};

                const auto root_dir = fs::path{"static"};

                std::ostringstream stream;
                size_t fileSize;
                server.serve_static_file(root_dir, request.resource, stream, fileSize);

                auto body = stream.str();
                // httpResponse.httpMessage = body.data();
                // httpResponse.httpMessageLength = fileSize;

                // httpResponse.buildResponseBody(body);
                httpResponse.httpMessageLength = body.size();
                httpResponse.httpResponseBody = body;
                httpResponse.httpMessage = httpResponse.httpResponseBody.data();


                std::cout << "BODY: \n" << httpResponse.httpMessage << std::endl;

                return httpResponse;
            };

    // Creating callback function
    auto helloWorld = [](const HttpRequest_t &request) -> HttpResponse_t
    {
        // Creating Response Object with StatusCode OK -> 200
        HttpResponse_t httpResponse{HttpResponse_t::HttpStatusCode::Ok};

        // Setting some headers
        httpResponse.setHeader("Server", "Sandi");
        httpResponse.setHeader("Content-Type", "text/plain");

        // Preparing answer from server
        std::string resp_msg = "Hello World from Sandi's Server";

        // Building Body
        httpResponse.buildResponseBody(resp_msg);

        return httpResponse;
    };

    // Creating callback function
    auto home = [](const HttpRequest_t &request) -> HttpResponse_t
    {
        // Creating Response Object with StatusCode OK -> 200
        HttpResponse_t httpResponse{HttpResponse_t::HttpStatusCode::Ok};

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
    auto dummy = [](const HttpRequest_t &request) -> HttpResponse_t
    {
        // Creating Response Object with StatusCode OK -> 200
        HttpResponse_t httpResponse{HttpResponse_t::HttpStatusCode::Ok};

        // Setting some headers
        httpResponse.setHeader("Server", "Sandi");
        httpResponse.setHeader("Content-Type", "text/html");

        // Preparing answer from server
        std::string resp_msg = "<h1>DUMMY</h1>";

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

    server.registerRequestHandler("/static/", HttpRequest_t::HttpMethode::GET,
                                  serveStatic);

    server.startServer("127.0.0.1", 8000);
    return 0;
}
