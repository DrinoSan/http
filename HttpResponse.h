//
// Created by becirbeg on 28.10.2022.
//

#ifndef SIMPLEHTTPSERVER2_HTTPRESPONSE_H
#define SIMPLEHTTPSERVER2_HTTPRESPONSE_H

#include <string>
#include "HttpMessage.h"

class HttpResponse_t : public HttpMessage {

  public:
   enum class HttpStatusCode {
      Continue = 100,
      SwitchingProtocols = 101,
      EarlyHints = 103,
      Ok = 200,
      Created = 201,
      Accepted = 202,
      NonAuthoritativeInformation = 203,
      NoContent = 204,
      ResetContent = 205,
      PartialContent = 206,
      MultipleChoices = 300,
      MovedPermanently = 301,
      Found = 302,
      NotModified = 304,
      BadRequest = 400,
      Unauthorized = 401,
      Forbidden = 403,
      NotFound = 404,
      MethodNotAllowed = 405,
      RequestTimeout = 408,
      ImATeapot = 418,
      InternalServerError = 500,
      NotImplemented = 501,
      BadGateway = 502,
      ServiceUnvailable = 503,
      GatewayTimeout = 504,
      HttpVersionNotSupported = 505,
   };

   HttpResponse_t(HttpStatusCode code);
   HttpResponse_t();
   HttpStatusCode statusCode;
   void setHeader(const std::string& key, const std::string& value);
   std::string httpStatusCodeToString(HttpStatusCode code);
   void buildResponseBody(std::string respMsg);
};

#endif  // SIMPLEHTTPSERVER2_HTTPRESPONSE_H
