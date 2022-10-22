cmake_minimum_required(VERSION 3.21)
project(simpleHttpServer2)

set(CMAKE_CXX_STANDARD 17)

add_executable(simpleHttpServer2 main.cpp simpleHttpServer.cpp simpleHttpServer.h)
if(WIN32)
    target_link_libraries(simpleHttpServer2 wsock32 ws2_32)
endif()
