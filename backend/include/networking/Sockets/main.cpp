#include "ListeningSocket.hpp"
#include <iostream>

int main()
{
    std::cout << "Starting..." << std::endl;
    std::cout << "Binding Socket..." << std::endl;
    HDE::BindingSocket bs = HDE::BindingSocket(AF_INET, SOCK_STREAM, 0, 81, INADDR_ANY);
    std::cout << "Listening Socket..." << std::endl;
    HDE::ListeningSocket ls = HDE::ListeningSocket(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10);
    std::cout << "Success!" << std::endl;
}