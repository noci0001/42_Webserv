#ifndef SIMPLESOCKET_HPP
#define SIMPLESOCKET_HPP

#include "SimpleSocket.hpp"
#include <netinet/in.h>

namespace HDE {
    class SimpleSocket {
    private:
        struct sockaddr_in address;
        int sock;
        int connection;

    public:
        SimpleSocket(int domain, int service, int protocol, int port, long interface);

        virtual int connect_to_network(int sock, struct sockaddr_in address) = 0;

        void test_connection(int);

        struct sockaddr_in get_address();

        int get_sock();

        int get_connection();

        void set_connection(int con);
    };
}


#endif
