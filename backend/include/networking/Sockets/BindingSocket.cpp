#include "BindingSocket.hpp"

HDE::BindingSocket::BindingSocket(int domain, int service, int protocol, int port, long interface) : SimpleSocket(domain, service, protocol, port, interface) {
    SimpleSocket::set_connection(connect_to_network(SimpleSocket::get_sock(), SimpleSocket::get_address()));
    SimpleSocket::test_connection(SimpleSocket::get_connection());
}

int HDE::BindingSocket::connect_to_network(int sock, struct sockaddr_in address) {
    return bind(sock, (struct sockaddr *)&address, sizeof(address));
}
