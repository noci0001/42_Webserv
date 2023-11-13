//
// Created by Samuel Nocita on 10/30/23.
//

#include "TestServer.hpp"
#include <iostream>
#include "../Webserv.hpp"
#include <fstream>
#include <sstream>

HDE::TestServer::TestServer(): SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10){
    HDE::TestServer::launch();
}

// If there is nothing trying to get accepted
// it is going to idle thus the loop is not
// going to be infinitely running
void HDE::TestServer::accepter() {
    struct sockaddr_in address = get_socket()->get_address();
    int addrlen = sizeof(address);
    new_socket = accept(get_socket()->get_sock(), (struct sockaddr *) &address, (socklen_t *) &addrlen);
    read(new_socket, buffer, 30000);
}

char *return_content(std::string path) {
    if (path == "/")
        path = "/index.html";
    std::string prefix = "../../root";
    path = prefix + path;
    std::cout << path << std::endl;
    char *buffer = new char[3000];
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Error opening file!" << path << std::endl;
        return NULL;
    }
    if (!file.read(buffer, 3000)) {
        std::cerr << "Error handling file!" << path << std::endl;
        delete[] buffer;
        file.close();
        return (NULL);
    }
    file.close();
    buffer[3000] = '\0';
    return (buffer);
}

std::string analyze_request(char *request) {
    std::vector<std::string> words;
    std::istringstream stream(request);
    std::string firstLine;
    std::string word;

    std::cout << request << std::endl;
    if (std::getline(stream, firstLine)) {
        std::cout << "First line: " << firstLine << std::endl;
    }
    std::istringstream first_line_stream(firstLine);
    while (first_line_stream >> word) {
        words.push_back(word);
    }
    std::cout << "REQUESTING PATH =>";
    std::cout << words[1] << std::endl;
    return (words[1]);
}

void HDE::TestServer::handler(){
    currect_request_url = analyze_request(buffer);
}

void HDE::TestServer::responder(){
    std::cout << "RESPONDING: ->";
    char *responding = return_content(currect_request_url);
    write(new_socket, responding, strlen(responding));
    close(new_socket);

}

void HDE::TestServer::launch(){
    while (true)
    {
        std::cout << "\t === Waiting for new connection ===" << std::endl;
        accepter();
        handler();
        responder();
        std::cout << " === DONE ===" << std::endl;
    }
}