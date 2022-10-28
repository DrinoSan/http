cmake_minimum_required(VERSION 3.16)
project(simpleHttpServer2)

set(CMAKE_CXX_STANDARD 17)

add_executable(simpleHttpServer2 main.cpp simpleHttpServer.cpp simpleHttpServer.h HttpMessage.cpp HttpMessage.h HttpRequest.cpp HttpRequest.h HttpResponse.cpp HttpResponse.h)
