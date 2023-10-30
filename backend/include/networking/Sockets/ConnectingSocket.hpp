#ifndef INC_42_WEBSERV_CONNECTINGSOCKET_HPP
#define INC_42_WEBSERV_CONNECTINGSOCKET_HPP

#include "./ConnectingSocket.hpp"

class ConnectingSocket {
public:
    ConnectingSocket(int domain, int service, int protocol, int port, u_long interface);
    int connect_to_network(int sock, struct sockaddr_in address);
};


#endif //INC_42_WEBSERV_CONNECTINGSOCKET_HPP
