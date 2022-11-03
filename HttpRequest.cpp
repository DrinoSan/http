//
// Created by becirbeg on 22.10.2022.
//

#include "HttpRequest.h"

std::string HttpRequest_t::httpMethodeToString(HttpRequest_t::HttpMethode methode) {
  switch (methode) {
  case HttpRequest_t::HttpMethode::GET:
    return "GET";
  case HttpRequest_t::HttpMethode::POST:
    return "POST";
  case HttpRequest_t::HttpMethode::DELETE:
    return "DELETE";
  case HttpRequest_t::HttpMethode::UPDATE:
    return "UPDATE";
  case HttpRequest_t::HttpMethode::PUT:
    return "PUT";
  default:
    return "UNKNOWN METHODE TYPE ERROR";
  }

  return "Unknown Methode Type!";
}

HttpRequest_t::HttpMethode HttpRequest_t::stringToHttpMethode(std::string methode) {
  if (methode == "GET")
    return HttpRequest_t::HttpMethode::GET;
  else if (methode == "POST")
    return HttpRequest_t::HttpMethode::POST;
  else if (methode == "DELETE")
    return HttpRequest_t::HttpMethode::DELETE;
  else if (methode == "UPDATE")
    return HttpRequest_t::HttpMethode::UPDATE;
  else if (methode == "PUT")
    return HttpRequest_t::HttpMethode::PUT;
  else
    return HttpRequest_t::HttpMethode::UNKNOWN;
}

// Constructor
HttpRequest_t::HttpRequest_t(char *msg) : HttpMessage{msg} {
}
