//
// Created by Samuel Nocita on 10/30/23.
//

#include "ListeningSocket.hpp"

HDE::ListeningSocket::ListeningSocket(int domain, int service, int protocol, int port, long interface, int bklg) : BindingSocket(domain, service, protocol, port, interface) {
    backlog = bklg;
    start_listening();
    test_connection(listening);
}

void HDE::ListeningSocket::start_listening() {
    listening = listen(HDE::SimpleSocket::get_sock(), backlog);
}