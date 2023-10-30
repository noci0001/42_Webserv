#ifndef INC_42_WEBSERV_LISTENINGSOCKET_HPP
#define INC_42_WEBSERV_LISTENINGSOCKET_HPP

#include "BindingSocket.hpp"

class ListeningSocket: public BindingSocket {
private:
    int backlog;
    int listening;
public:
    ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int bklg);
    void start_listening();
};


#endif //INC_42_WEBSERV_LISTENINGSOCKET_HPP
