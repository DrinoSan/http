// System Headers
#include <cstddef>
#include <iostream>
#include <string_view>
#include <utility>
#include <sstream>

// Project Headers
#include "HttpParser.h"
#include "HttpRequest.h"

//----------------------------------------------------------------------------
void HttpParser_t::mapHeaders(HttpRequest_t& httpReq)
{
    // First line is METHODE, PATH, VERSION aka Request line
    // Remaining lines are the headers
    std::istringstream stream( httpReq.buffer );
    std::string tester = httpReq.buffer;
    std::string method, path, version;

    // Parsing methode, path and version
    stream >> method >> path >> version;

    // Converting methode to enum
    httpReq.httpMethode = httpReq.stringToHttpMethode(method);
    
    httpReq.httpUri = path;
    httpReq.httpVersion = version;

    auto requestMessage = stream.str();

    // Declaring variables for positions of key and values
    std::string::size_type key_pos = 0;
    std::string::size_type key_end;
    std::string::size_type val_pos;
    std::string::size_type val_end;

    auto end_of_headers = tester.find("\r\n\r\n");
    // Finding end of httpMessage
    if( end_of_headers == std::string::npos )
    {
        // Todo: What shall we do here?
        return;
    }

    while((key_end = requestMessage.find(':', key_pos)) != std::string::npos && key_end < end_of_headers)
    {
        if((val_pos = requestMessage.find_first_not_of(": ", key_end)) == std::string::npos)
        {
            break;
        }

        val_end = requestMessage.find("\n", val_pos);
        httpReq.headers.emplace(requestMessage.substr(key_pos, key_end - key_pos), requestMessage.substr(val_pos, val_end - val_pos - 1));

        key_pos = val_end;
        if(key_pos != std::string::npos)
        {
            ++key_pos;
        }
    }

    auto it = httpReq.headers.find("Content-Length");
    if( it == httpReq.headers.end() )
    {
        return;
    }


    // Adding 4 to the position of end_of_headers because \r\n\r\n
    end_of_headers += 4;

    // Let's trust, that the content_length is correct
    // Constructing std::string from a char array.
    // We start at the 0th position and add end_of_headers <--- this is our START
    // We get all chars till start + content_length        <--- this is our END
    //                  //_____________START_______________,_________________________________END_______________________________________
    std::string bodyMessage(httpReq.buffer + end_of_headers, httpReq.buffer + end_of_headers + (size_t) + std::stoi(it->second));

    httpReq.httpBody = bodyMessage;
}

//----------------------------------------------------------------------------
//char* HttpParser_t::parseMethodePathVersion(HttpRequest_t* httpReq)
//{
//    try
//    {
//        char* buf = httpReq->buffer;
//        const char* msg_end = "\r";
//
//        char* head = buf;
//        char* tail = buf;
//
//        // Find request type
//        while (tail != msg_end && *tail != ' ')
//        {
//            ++tail;
//        }
//        // httpHeaders["Type"] = std::string(head, tail);
//        HttpRequest_t::HttpMethode methode =
//                httpReq->stringToHttpMethode(std::string(head, tail));
//
//        if (methode == HttpRequest_t::HttpMethode::UNKNOWN)
//        {
//            return nullptr;
//        }
//        httpReq->httpMethode = methode;
//
//        // We need to increment tail because it is currently on the whitspace
//        head = tail++;
//
//        // Find path
//        while (tail != msg_end && *tail != ' ')
//        {
//            ++tail;
//        }
//        httpReq->httpUri = std::string(++head, tail);
//
//        // Find HTTP version
//        while (tail != msg_end && *tail == ' ')
//        {
//            ++tail;
//        }
//        head = tail;
//
//        while (tail != msg_end && *tail != '\r')
//        {
//            ++tail;
//        }
//        httpReq->httpVersion = std::string(head, tail);
//
//        // To skip \r\n
//        char* buffer = httpReq->buffer;
//        buffer = tail + 2;
//
//        return buffer;
//    } catch (...)
//    {
//        std::cout << "Header is definitely not correct" << std::endl;
//        return nullptr;
//    }
//}
//
////----------------------------------------------------------------------------
//bool HttpParser_t::parseRequest(HttpRequest_t* httpReq,
//                                std::map<std::string, std::string> &headers)
//{
//    char* begin;
//    char* end;
//    char* buffer = parseMethodePathVersion(httpReq);
//
//    if (buffer == nullptr)
//    {
//        std::cout << "GOT A NULLPTR. CLIENT ASKS FOR UNKNOWN SHIT" << std::endl;
//        std::cout << "Probably the Header is just wrong!" << std::endl;
//        return false;
//    }
//
//    for (size_t i = 0; i < NUM_HTTP_HEADERS; ++i)
//    {
//        // *(buffer++) |= 32 is a way to make everything lowercase
//        for (begin = buffer;
//             (*buffer != ':') && (*(unsigned char*) buffer) > 32;)
//        {
//            if (*(buffer) == '_')
//            {
//                buffer++;
//                continue;
//            }
//            *(buffer++) |= 32;
//        }
//        std::string key{std::string(begin, (size_t) (buffer - begin))};
//
//        // Checking if space is after :
//        if (buffer[0] == ':' && buffer[1] == ' ')
//        {
//            buffer += 2;
//        } else
//        {
//            // std::cout << *buffer << *(buffer+1) << *(buffer+2) << *(buffer+3) <<
//            // std::endl;
//            std::cout << "Headers are not correctly formated!" << std::endl;
//            break;
//        }
//
//        begin = buffer;
//
//        while (*buffer != '\r')
//        {
//            ++buffer;
//        }
//        end = buffer;
//
//        bool found{false};
//        retry:
//        if (*end == '\r')
//        {
//            if (*(end + 1) == '\n')
//            {
//                end += 1;
//                ++buffer;
//                if (*(end + 1) == '\r' || found)
//                {
//                    if (found)
//                    {
//                        return true;
//                        // break;
//                    }
//                    found = true;
//                }
//                ++end;
//                goto retry;
//            }
//        }
//
//        headers[key] = std::string{begin, (size_t) (buffer - begin)};
//        buffer += 1;
//    }
//
//    return false;
//}
