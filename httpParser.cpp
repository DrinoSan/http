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

bool HttpParser::parseMethodePathVersion(HttpRequest *httpReq) {

  char *begin, *colon, *end = httpReq->httpRequestBlob;
  char *buf = httpReq->httpRequestBlob;
  const char *msg_end = "\r";
  const char *new_line = "\n";

  char *head = buf;
  char *tail = buf;

  // Find request type
  while (tail != msg_end && *tail != ' ')
    ++tail;
  // httpHeaders["Type"] = std::string(head, tail);
  HttpRequest::HttpMethode methode = httpReq->stringToHttpMethode(std::string(head, tail));
  if( methode == HttpRequest::HttpMethode::UNKNOWN )
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
  httpReq->httpRequestBlob = tail + 2;

  //std::cout << "Path: " << httpReq->httpUri << std::endl;
  //std::cout << "METHODE: " << httpReq->httpMethodeToString(httpReq->httpMethode) << std::endl;
  return true;
}

bool HttpParser::parseRequest(HttpRequest *httpReq,
                              std::map<std::string, std::string>& headers) {

  char *begin, *end = httpReq->httpRequestBlob;

  parseMethodePathVersion(httpReq);
  //for (size_t i = 0; i < NUM_HTTP_HEADERS; ++i) {
    //// *(httpMessageBlob++) |= 32 is a way to make everything lowercase
    //for (begin = httpReq->httpRequestBlob;
         //(*httpReq->httpRequestBlob != ':') &&
         //(*(unsigned char *)httpReq->httpRequestBlob) > 32;
         //*(httpReq->httpRequestBlob++) |= 32)
      //;
    //std::string key{std::string(begin, (size_t)(httpReq->httpRequestBlob - begin) )};
    ////headers->key =
        ////std::string_view(begin, (size_t)(httpReq->httpRequestBlob - begin));

    //// Checking if space is after :
    //if (httpReq->httpRequestBlob[0] == ':' &&
        //httpReq->httpRequestBlob[1] == ' ') {
      //httpReq->httpRequestBlob += 2;
    //} else {
      //std::cout << "Headers are not correctly formated!" << std::endl;
      //break;
    //}

    //begin = httpReq->httpRequestBlob;

    //while (*httpReq->httpRequestBlob != '\r')
      //++httpReq->httpRequestBlob;
    //end = httpReq->httpRequestBlob;
  //retry:
    //if (*end == '\r') {
      //if (*(end + 1) == '\n') {
        //end += 1;
        //++httpReq->httpRequestBlob;
        //if (*(end + 1) == '\0') {
          //std::cout << "FINISHED PARSING!!!!" << std::endl;
          //break;
        //}
        //++end;
        //goto retry;
      //}
    //}

    //headers[key] = std::string{begin, (size_t)(httpReq->httpRequestBlob - begin)};
    ////headers->value =
        ////std::string_view(begin, (size_t)(httpReq->httpRequestBlob - begin));
    //httpReq->httpRequestBlob += 1;

    //std::cout << "KEY  : " << key << std::endl;
    //std::cout << "Value: " << headers[key] << std::endl;
    //std::cout << "----------------" << std::endl;
  //}

  return false;
}
