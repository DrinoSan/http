//
// Created by becirbeg on 28.10.2022.
//

#include "HttpResponse.h"
#include <iostream>

HttpResponse::HttpResponse(HttpResponse::HttpStatusCode code)
    : statusCode{code} {}

void HttpResponse::setHeader(const std::string &key, const std::string &value) {
  headers[key] = std::move(value);
}

std::string HttpResponse::httpStatusCodeToString(HttpResponse::HttpStatusCode code) {
  switch (code) {
  case HttpResponse::HttpStatusCode::Ok:
    return "OK";
  default:
    return "Ok";
  }
}

void HttpResponse::buildResponseBody(std::string resMsg) {
  // std::string resp = "HTTP/1.1 200 OK\r\n"
  //"Server: Hello\r\n"
  //"Content-Length: 23\r\n"
  //"Content-Type: text/plain\r\n"
  //"\r\n"
  //"Hello, world!!!tester!\n";
  //
  //
  // HTTP/1.1 200 OK
  // Content-Length: 31
  // Content-Type: text/plain
  // Server: Sandi

  // Hello World from Sandi's Server

  std::string end{"\r\n"};
  httpResponseBody =
      httpVersion + " " + "200" + " " + httpStatusCodeToString(statusCode) + end;
  setHeader("Content-Length", std::to_string(resMsg.size()));
  for (auto &[key, val] : headers) {
    httpResponseBody += key + ": " + val + end;
  }
  httpResponseBody += end;
  httpResponseBody += resMsg;

  httpMessageLength = httpResponseBody.size();
  httpMessage = httpResponseBody.data();
}
