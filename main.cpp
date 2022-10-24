#include <cstdlib>
#include <iostream>

#include "simpleHttpServer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    simpleHttpServer server;

    if( ! server.startServer( "0.0.0.0", 9000 ) ) {
        std::cout << "PORT: 8000" << std::endl;
        server.startServer("0.0.0.0", 8000);
    }

    return 0;
}
