//
// Created by becirbeg on 28.10.2022.
//

#include "HttpResponse.h"
#include <iostream>

HttpResponse_t::HttpResponse_t(HttpResponse_t::HttpStatusCode code)
    : statusCode{code} {}

HttpResponse_t::HttpResponse_t()
    : statusCode{HttpResponse_t::HttpStatusCode::Ok} {}

void HttpResponse_t::setHeader(const std::string& key,
                               const std::string& value) {
   headers[key] = std::move(value);
}

std::string HttpResponse_t::httpStatusCodeToString(
    HttpResponse_t::HttpStatusCode code) {
   switch (code) {
      case HttpResponse_t::HttpStatusCode::Ok:
         return "OK";
      default:
         return "Ok";
   }
}

void HttpResponse_t::buildResponseBody(std::string resMsg) {
   std::string end{"\r\n"};
   httpResponseBody = httpVersion + " " + "200" + " " +
                      httpStatusCodeToString(statusCode) + end;
   setHeader("Content-Length", std::to_string(resMsg.size()));
   for (auto& [key, val] : headers) {
      httpResponseBody += key + ": " + val + end;
   }
   httpResponseBody += end;
   httpResponseBody += resMsg;

   httpMessageLength = httpResponseBody.size();
   httpMessage = httpResponseBody.data();
}
