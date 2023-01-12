cmake_minimum_required(VERSION 3.16)
project(simpleHttpServer2)

set(CMAKE_CXX_STANDARD 17)


find_package(OpenSSL REQUIRED)

# this is in the .zsrhc exported
include_directories(${OPENSSL_INCLUDE_DIR})

file(GLOB SimpleHttpserverFiles_SRC
        "*.h"
        "*.cpp"
        )

add_executable(simpleHttpServer2 ${SimpleHttpserverFiles_SRC})
target_link_libraries(simpleHttpServer2 ${OPENSSL_LIBRARIES})
