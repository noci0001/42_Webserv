#include "../include/Webserv.hpp"

SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, u_long interface) {
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    //ESTABLISHING CONNECTION
    this->sock = socket(domain, service, protocol);
    test_connection(this->sock);
}

void SimpleSocket::test_connection(int item_to_test) {
    if (item_to_test < 0) {
        std::cerr << "Failed to connect.\nError: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in SimpleSocket::get_address() {
    return (this->address);
}

int SimpleSocket::get_sock() {
    return (this->sock);
}

int SimpleSocket::get_connection() {
    return (this->connection);
}

SimpleSocket::~SimpleSocket() {
    close(this->sock);
    close(this->connection);
}

void SimpleSocket::set_connection(int con) {
    this->connection = con;
}