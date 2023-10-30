//
// Created by Samuel Nocita on 10/30/23.
//

#include "ListeningSocket.hpp"

ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, u_long interface, int bklg) : BindingSocket(domain, service, protocol, port, interface) {
    backlog = bklg;
    ListeningSocket::start_listening();
    SimpleSocket::test_connection(listening);
}

void ListeningSocket::start_listening() {
    listening = listen(SimpleSocket::get_connection(), backlog);
}