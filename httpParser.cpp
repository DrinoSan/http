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

// GET /sand HTTP/1.1
// Host: 127.0.0.1:8000
// Upgrade-Insecure-Requests: 1
// Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*[>;q=0.8
// User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7)
// AppleWebKit/605.1.15 (KHTML, like Gecko) Version/16.0 Safari/605.1.15
// Accept-Language: en-GB,e__objc_imageinfo

//----------------------------------------------------------------------------
char* HttpParser_t::parseMethodePathVersion(HttpRequest_t* httpReq) {

   std::cout << "Entering parse Methode" << std::endl;
   char *begin, *colon, *end, *buffer = httpReq->buffer;
   char* buf = httpReq->buffer;
   const char* msg_end = "\r";
   const char* new_line = "\n";

   char* head = buf;
   char* tail = buf;

   // Find request type
   while (tail != msg_end && *tail != ' ')
      ++tail;
   // httpHeaders["Type"] = std::string(head, tail);
   HttpRequest_t::HttpMethode methode =
       httpReq->stringToHttpMethode(std::string(head, tail));
   if (methode == HttpRequest_t::HttpMethode::UNKNOWN)
      return nullptr;
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
   buffer = tail + 2;

   std::cout << "Exiting parse Methode" << std::endl;
   return buffer;
}

//----------------------------------------------------------------------------
bool HttpParser_t::parseRequest(HttpRequest_t* httpReq,
                                std::map<std::string, std::string>& headers) {
   char *begin, *end, *buffer = parseMethodePathVersion(httpReq);
   if (buffer == nullptr) {
      std::cout << "GOT A NULLPTR. CLIENT ASKS FOR UNKNOWN SHIT" << std::endl;
      return false;
   }
   for (size_t i = 0; i < NUM_HTTP_HEADERS; ++i) {
      // *(httpMessageBlob++) |= 32 is a way to make everything lowercase
      for (begin = buffer;
           (*buffer != ':') && (*(unsigned char*)buffer) > 32;) {
         if (*(buffer) == '_') {
            buffer++;
            continue;
         }
         *(buffer++) |= 32;
      }
      std::string key{std::string(begin, (size_t)(buffer - begin))};

      // Checking if space is after :
      if (buffer[0] == ':' && buffer[1] == ' ') {
         buffer += 2;
      } else {
         // std::cout << *buffer << *(buffer+1) << *(buffer+2) << *(buffer+3) <<
         // std::endl;
         std::cout << "Headers are not correctly formated!" << std::endl;
         break;
      }

      begin = buffer;

      while (*buffer != '\r') {
         ++buffer;
      }
      end = buffer;

      bool found{false};
   retry:
      if (*end == '\r') {
         if (*(end + 1) == '\n') {
            end += 1;
            ++buffer;
            if (*(end + 1) == '\r' || found) {
               if (found) {
                  return true;
                  // break;
               }
               found = true;
            }
            ++end;
            goto retry;
         }
      }

      headers[key] = std::string{begin, (size_t)(buffer - begin)};
      buffer += 1;
   }

   return false;
}
