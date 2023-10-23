#ifndef WEBSERV_HPP
#define WEBSERV_HPP

// 0: SUCCESS
#define SUCCESS 0

// -1: To Be Determined -> This status indicates that the
// process has not yet to determine the result of the algorithm.
#define TBD -1

// 1: FAILURE
#define FAILURE 1

//CLIENT MAX BODY SIZE
#define MAX_CONTENT_LENGTH 420000


#include "Parsing.hpp"
#include "Configfile.hpp"
#include "Location.hpp"
#include "ParsingUtils.hpp"
//#include "ServerConfig.hpp"
//#include "Response.hpp"
#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <iomanip>
#include <cstring>
#include <utility>
#include <sstream>

//ServerConfig.hpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <map>
#include <unistd.h>
#include <sys/stat.h>
#define MAX_CONTENT_LENGTH 420000

//Utils.cpp
int ft_stoi(std::string str);
std::string statusCodeString(short statusCode);

//Colors
const std::string BOLD = "\033[1m";
const std::string RED = "\033[38;5;1m";
const std::string GREEN = "\033[38;5;2m";
const std::string YELLOW = "\033[38;5;3m";
const std::string RESET = "\033[0m";

static std::string serverParameter[] = { "listen ", "host ", "server_name ", "error_page ", "client_max_body_size ", "root" , "index ", "allow_methods"};

#endif