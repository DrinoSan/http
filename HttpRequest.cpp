//
// Created by becirbeg on 22.10.2022.
//

#include <iostream>

#include "HttpParser.h"
#include "HttpRequest.h"

std::string HttpRequest::httpMethodeToString(HttpRequest::HttpMethode methode) {
  switch (methode) {
  case HttpRequest::HttpMethode::GET:
    return "GET";
  case HttpRequest::HttpMethode::POST:
    return "POST";
  case HttpRequest::HttpMethode::DELETE:
    return "DELETE";
  case HttpRequest::HttpMethode::UPDATE:
    return "UPDATE";
  case HttpRequest::HttpMethode::PUT:
    return "PUT";
  default:
    return "UNKNOWN METHODE TYPE ERROR";
  }

  return "Unknown Methode Type!";
}

HttpRequest::HttpMethode HttpRequest::stringToHttpMethode(std::string methode) {
  if (methode == "GET")
    return HttpRequest::HttpMethode::GET;
  else if (methode == "POST")
    return HttpRequest::HttpMethode::POST;
  else if (methode == "DELETE")
    return HttpRequest::HttpMethode::DELETE;
  else if (methode == "UPDATE")
    return HttpRequest::HttpMethode::UPDATE;
  else if (methode == "PUT")
    return HttpRequest::HttpMethode::PUT;
  else
    return HttpRequest::HttpMethode::UNKNOWN;
}

// Constructor
HttpRequest::HttpRequest(char *msg) : HttpMessage{msg} {}
