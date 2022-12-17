#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "HttpRequest_t.h"
#include "HttpResponse_t.h"
#include "SimpleHttpServer.h"
#include "JsonParser_t.h"

using namespace rapidjson;

int main()
{

    SimpleHttpServer_t server;

    auto serveStatic =
            [&server](const HttpRequest_t &request) -> HttpResponse_t
            {
                HttpResponse_t httpResponse{HttpResponse_t::HttpStatusCode::Ok};

                // the root_dir path depends on where the binary is executed
                const auto root_dir = fs::path{"../static"};

                std::ostringstream stream;
                size_t fileSize;
                server.serve_static_file(root_dir, request.resource, stream, fileSize, httpResponse);

                auto body = stream.str();

                httpResponse.httpMessageLength = body.size();
                httpResponse.httpResponseBody = body;
                httpResponse.httpMessage = httpResponse.httpResponseBody;

                httpResponse.setHeader("Server", "Sandi");
                httpResponse.buildResponseBody(body);

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

    auto testJsonParsing = [](const HttpRequest_t &request) -> HttpResponse_t
    {
        // Creating Response Object with StatusCode OK -> 200
        HttpResponse_t httpResponse{HttpResponse_t::HttpStatusCode::Ok};

        // Setting some headers
        httpResponse.setHeader("Server", "Sandi");
        httpResponse.setHeader("Content-Type", "text/plain");

        // Preparing answer from server
        std::string resp_msg = "Hello World from Sandi's Server with Hassan's help";
        for( const auto& [key, value] : request.headers )
        {
            std::cout << "HEADER Key: " << key << " Value: " << value << std::endl;
        }

        if( ! request.httpBody.empty() )
        {
            std::cout << "Message Body: \n" << request.httpBody << std::endl;
            // 1. Parse a JSON string into DOM.
//            const char* json = request.httpBody.c_str();
//            Document d;
//            d.Parse(json);

//            const rapidjson::Value &V{};
//            for (auto it = d.MemberBegin(); it != d.MemberEnd(); ++it)
//            {
//                std::cout << "Key: " << it->name.GetString() << std::endl;
//                std::cout << "Value: " << it->value.GetDouble() << std::endl;
//                std::cout << "------" << std::endl;
//            }
            JsonParser_t jP(request.httpBody);
            std::string type = jP.getTypeOfElement("productId");
            std::cout << "TYPE:::::: " << type << std::endl;

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

    server.registerRequestHandler("/jsonParse", HttpRequest_t::HttpMethode::POST, testJsonParsing);

    server.startServer("127.0.0.1", 8000);
    return 0;
}
