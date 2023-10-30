#ifndef INC_42_WEBSERV_BINDINGSOCKET_HPP
#define INC_42_WEBSERV_BINDINGSOCKET_HPP

#include "SimpleSocket.hpp"

namespace HDE {
    class BindingSocket : public SimpleSocket {
    public:
        BindingSocket(int domain, int service, int protocol, int port, long interface);

        int connect_to_network(int sock, struct sockaddr_in address);
    };
}
#endif //INC_42_WEBSERV_BINDINGSOCKET_HPP
