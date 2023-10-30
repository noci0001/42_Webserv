#ifndef LISTENINGSOCKET_HPP
#define LISTENINGSOCKET_HPP

#include "BindingSocket.hpp"

namespace HDE {
    class ListeningSocket : public BindingSocket {
    private:
        int backlog;
        int listening;
    public:
        ListeningSocket(int domain, int service, int protocol, int port, long interface, int bklg);

        void start_listening();
    };
}

#endif //INC_42_WEBSERV_LISTENINGSOCKET_HPP
