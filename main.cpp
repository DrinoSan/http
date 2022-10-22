#include <iostream>

#include "simpleHttpServer.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    simpleHttpServer server;

    server.startServer("0.0.0.0", 8000);

    return 0;
}
