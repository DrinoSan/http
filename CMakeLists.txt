cmake_minimum_required(VERSION 3.16)
project(simpleHttpServer2)

set(CMAKE_CXX_STANDARD 17)

include_directories(/opt/homebrew/Cellar/openssl@3/3.0.7/include)

file(GLOB SimpleHttpserverFiles_SRC
        "*.h"
        "*.cpp"
        )

add_executable(simpleHttpServer2 ${SimpleHttpserverFiles_SRC})
