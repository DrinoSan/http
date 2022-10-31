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
