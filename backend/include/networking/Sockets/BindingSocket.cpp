#include "BindingSocket.hpp"
#include "../../Webserv.hpp"

BindingSocket::BindingSocket(int domain, int service, int protocol, int port, u_long interface) : SimpleSocket(domain, service, protocol, port, interface) {
    set_connection(connect_to_network(SimpleSocket::get_sock(), SimpleSocket::get_address()));
    test_connection(SimpleSocket::get_connection());
}

int BindingSocket::connect_to_network(int sock, struct sockaddr_in address) {
    return bind(sock, (struct sockaddr *)&address, sizeof(address));
}
