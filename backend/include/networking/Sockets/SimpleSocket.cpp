#include "../../Webserv.hpp"

HDE::SimpleSocket::SimpleSocket(int domain, int service, int protocol, int port, long interface) {
    address.sin_family = domain;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(interface);
    //ESTABLISHING CONNECTION
    this->sock = socket(domain, service, protocol);
    test_connection(this->sock);
}

void HDE::SimpleSocket::test_connection(int item_to_test) {
    if (item_to_test < 0) {
        std::cerr << "Failed to connect.\nError: " << strerror(errno) << std::endl;
        exit(EXIT_FAILURE);
    }
}

struct sockaddr_in HDE::SimpleSocket::get_address() {
    return (this->address);
}

int HDE::SimpleSocket::get_sock() {
    return (this->sock);
}

int HDE::SimpleSocket::get_connection() {
    return (this->connection);
}

void HDE::SimpleSocket::set_connection(int con) {
    this->connection = con;
}