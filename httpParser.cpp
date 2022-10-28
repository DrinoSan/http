#include "httpParse.h"


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

bool HttpParser::parseHeaders(HttpRequest* httpReq) {
  char *begin, *colon, *end = httpReq->httpRequestBlob;
  char *buf = httpReq->httpRequestBlob;
  char *msg_end = "\r";
  char *new_line = "\n";

  char *head = buf;
  char *tail = buf;
  char *runner = buf;

  // Find request type
  while (tail != msg_end && *tail != ' ')
    ++tail;
  //httpHeaders["Type"] = std::string(head, tail);
  headers->key = std::string_view("Type", 4);
  headers->value = std::string_view(head, (size_t)(tail-head));
  headers++;

  // We need to increment tail because it is currently on the whitspace
  head = tail++;

  // Find path
  while (tail != msg_end && *tail != ' ')
    ++tail;
  headers->key = std::string_view("Path", 4);
  headers->value = std::string_view(head, (size_t)((++tail)-head));
  headers++;

  // Find HTTP version
  while (tail != msg_end && *tail == ' ')
    ++tail;
  head = tail;

  while (tail != msg_end && *tail != '\r')
    ++tail;
  headers->key = std::string_view("Version", 7);
  headers->value = std::string_view(head, (size_t)((tail)-head));
  headers++;

  httpMessageBlob = tail + 2;

  for (size_t i = 0; i < NUM_HTTP_HEADERS; ++i) {
      // *(httpMessageBlob++) |= 32 == toLowerCase
    for (begin = httpMessageBlob; (*httpMessageBlob != ':') && (*(unsigned char*)httpMessageBlob) > 32; *(httpMessageBlob++) |= 32);
    headers->key = std::string_view(begin, (size_t)(httpMessageBlob - begin)); 


    std::string header(begin, httpMessageBlob);
    std::cout << "$" << header << std::endl;
    break;
  }

  return false;
}
