#include "SimpleSocket.hpp"
#include "../../Webserv.hpp"
#include "ConnectingSocket.hpp"


ConnectingSocket::ConnectingSocket(int domain, int service, int protocol, int port, u_long interface): SimpleSoket(domain, service, protocol, port, interface) {
    SimpleSocket::set_connection(connect_to_network(SimpleSocket::get_sock(), SimpleSocket::get_address()));
    SimpleSocket::test_connection(SimpleSocket::get_connection());
}

int ConnectingSocket::connect_to_network(int sock, struct sockaddr_in address) {
    return connect(sock, (struct sockaddr *)&address, sizeof(address));
}