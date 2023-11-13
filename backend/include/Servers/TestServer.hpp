#ifndef INC_42_WEBSERV_TESTSERVER_HPP
#define INC_42_WEBSERV_TESTSERVER_HPP

#include "SimpleServer.hpp"
#include <unistd.h>
#include <string>

namespace HDE {
class TestServer: public SimpleServer {
private:
    std::string currect_request_url;
    char buffer[30000] = {0};
    int new_socket;
    void accepter();
    void handler();
    void responder();
public:
    TestServer();
    void launch();
};
}

#endif //INC_42_WEBSERV_TESTSERVER_HPP
