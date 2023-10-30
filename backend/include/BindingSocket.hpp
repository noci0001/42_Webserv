#ifndef INC_42_WEBSERV_BINDINGSOCKET_HPP
#define INC_42_WEBSERV_BINDINGSOCKET_HPP

#include "./SimpleSocket.hpp"
#include "Webserv.hpp"

class BindingSocket: public SimpleSocket {
public:
    BindingSocket(int domain, int service, int protocol, int port, u_long interface);
    int connect_to_network(int sock, struct sockaddr_in address);
};


#endif //INC_42_WEBSERV_BINDINGSOCKET_HPP
