#ifndef INC_42_WEBSERV_SIMPLESERVER_HPP
#define INC_42_WEBSERV_SIMPLESERVER_HPP

#include "../networking/networking.hpp"

namespace HDE {
    class SimpleServer {
    private:
        ListeningSocket *socket;
        virtual void accepter() = 0;
        virtual void handler() = 0;
        virtual void responder() = 0;
    public:
        SimpleServer(int domain, int service, int protocol, int port, long inferface, int bklg);
        virtual void launch() = 0;
        ListeningSocket *get_socket();
    };
}


#endif //INC_42_WEBSERV_SIMPLESERVER_HPP
