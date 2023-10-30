//
// Created by Samuel Nocita on 10/30/23.
//

#include "TestServer.hpp"
#include <iostream>

HDE::TestServer::TestServer(): SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10){
    HDE::TestServer::launch();
}

// If there is nothing trying to get accepted
// it is going to idle thus the loop is not
// going to be infinitely running
void HDE::TestServer::accepter(){
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
    read(new_socket, buffer, 30000);
}

void HDE::TestServer::handler(){
    std::cout << buffer << std::endl;
}

void HDE::TestServer::responder(){
    char *hello = "Hello from server";
    write(new_socket, hello, strlen(hello));
    close(new_socket);

}

void HDE::TestServer::launch(){
    while (true)
    {
        std::cout << "\t === Waiting for new connection ===" << std::endl;
        accepter();
        handler();
        responder();
        std::cout << " === DONE ===" << std::endl;
    }
}