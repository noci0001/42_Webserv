#ifndef WEBSERV_HPP
#define WEBSERV_HPP

// 0: SUCCESS
#define SUCCESS 0

// -1: To Be Determined -> This status indicates that the
// process has not yet to determine the result of the algorithm.
#define TBD -1

// 1: FAILURE
#define FAILURE 1

#include "Parsing.hpp"
#include "Configfile.hpp"
#include "ParsingUtils.hpp"
#include "ServerConfig.hpp"
#include "Response.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <string>

//ServerConfig.hpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <map>
#include <unistd.h>
#define MAX_CONTENT_LENGTH 420000

//Utils.cpp
int ft_stoi(std::string str);


#endif