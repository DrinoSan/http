// System Headers
#include <cstddef>
#include <iostream>
#include <string_view>
#include <utility>

// Project Headers
#include "HttpParser.h"
#include "HttpRequest.h"

// const char *msg =
//"GET / HTTP/1.1\r\n"
//"Host: 192.241.213.46:6880\r\n"
//"Upgrade-Insecure-Requests: 1\r\n"
//"Accept: "
//"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
//"User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_12_3) "
//"AppleWebKit/602.4.8 (KHTML, like Gecko) Version/10.0.3 "
//"Safari/602.4.8\r\n"
//"Accept-Language: en-us\r\n"
//"Accept-Encoding: gzip, deflate\r\n"
//"Connection: keep-alive\r\n\r\n";
//

//----------------------------------------------------------------------------
bool HttpParser::parseMethodePathVersion(HttpRequest *httpReq) {

  char *begin, *colon, *end = httpReq->httpMessage;
  char *buf = httpReq->httpMessage;
  const char *msg_end = "\r";
  const char *new_line = "\n";

  char *head = buf;
  char *tail = buf;

  // Find request type
  while (tail != msg_end && *tail != ' ')
    ++tail;
  // httpHeaders["Type"] = std::string(head, tail);
  HttpRequest::HttpMethode methode =
      httpReq->stringToHttpMethode(std::string(head, tail));
  if (methode == HttpRequest::HttpMethode::UNKNOWN)
    return false;
  httpReq->httpMethode = methode;

  // We need to increment tail because it is currently on the whitspace
  head = tail++;

  // Find path
  while (tail != msg_end && *tail != ' ')
    ++tail;
  httpReq->httpUri = std::string(++head, tail);

  // Find HTTP version
  while (tail != msg_end && *tail == ' ')
    ++tail;
  head = tail;

  while (tail != msg_end && *tail != '\r')
    ++tail;
  httpReq->httpVersion = std::string(head, tail);

  // To skip \r\n
  httpReq->httpMessage = tail + 2;

  return true;
}

//----------------------------------------------------------------------------
bool HttpParser::parseRequest(HttpRequest *httpReq,
                              std::map<std::string, std::string> &headers) {

  char *begin, *end = httpReq->httpMessage;

  parseMethodePathVersion(httpReq);
  for (size_t i = 0; i < NUM_HTTP_HEADERS; ++i) {
    // *(httpMessageBlob++) |= 32 is a way to make everything lowercase
    for (begin = httpReq->httpMessage;
         (*httpReq->httpMessage != ':') &&
         (*(unsigned char *)httpReq->httpMessage) > 32;
         *(httpReq->httpMessage++) |= 32)
      ;
    std::string key{
        std::string(begin, (size_t)(httpReq->httpMessage - begin))};
    // headers->key =
    // std::string_view(begin, (size_t)(httpReq->httpRequestBlob - begin));

    // Checking if space is after :
    if (httpReq->httpMessage[0] == ':' &&
        httpReq->httpMessage[1] == ' ') {
      httpReq->httpMessage += 2;
    } else {
      std::cout << "Headers are not correctly formated!" << std::endl;
      break;
    }

    begin = httpReq->httpMessage;

    while (*httpReq->httpMessage != '\r')
      ++httpReq->httpMessage;
    end = httpReq->httpMessage;

    bool found{false};
  retry:
    if (*end == '\r') {
      if (*(end + 1) == '\n') {
        end += 1;
        ++httpReq->httpMessage;
        if (*(end + 1) == '\r' || found) {
          if (found) {
            std::cout << "FINISHED PARSING SUCCESSFULLY!" << std::endl;
            //for(auto& [key, val] : headers) {
              //std::cout << key << " : " << val << std::endl;
            //}
            break;
          }
          found = true;
        }
        ++end;
        goto retry;
      }
    }

    headers[key] =
        std::string{begin, (size_t)(httpReq->httpMessage - begin)};
    // headers->value =
    // std::string_view(begin, (size_t)(httpReq->httpRequestBlob - begin));
    httpReq->httpMessage += 1;

  }

  return false;
}
