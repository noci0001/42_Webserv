#pragma once
#ifndef WEBSERV_HPP
# define WEBSERV_HPP

//CLIENT MAX BODY SIZE
#define MAX_CONTENT_LENGTH 420000

//CONNECTION TIMEOUT -> time in seconds after which the connection to client will be closed
#define CONNECTION_TIMEOUT 60

//MESSAGE_BUFFER_SIZE
#define MESSAGE_BUFFER_SIZE 42000

//Max Length of URI
#define MAX_URI_LENGTH 4096

#include <iostream>
#include <vector>
#include <map>
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

//ConsoleLog.hpp
#include <fcntl.h>
#include <sys/time.h>

//Response.hpp
#include <algorithm>
#include <iostream>
#include <string>
#include <map>
#include <string>

#include "Configfile.hpp"
#include "Location.hpp"
#include "ParsingUtils.hpp"
#include "ServerConfig.hpp"
#include "ServerControler.hpp"
#include "Response.hpp"
#include "ConsoleLog.hpp"
#include "HttpRequest.hpp"
#include "CgiHandler.hpp"
#include "ClientSide.hpp"
#include "MimeType.hpp"
#include "StatusCode.hpp"

//Utils.cpp
#include <dirent.h>
int ft_stoi(std::string str);
std::string statusCodeString(short statusCode);
std::string getErrorPage(short statusCode);
int	buildHtmlIndex(std::string &dir_name, std::vector<uint8_t> &body, size_t &body_length);
unsigned int fromHexToDecimal(const std::string &decimal);

//Colors
const std::string BOLD = "\033[1m";
//const std::string RED = "\033[38;5;1m";
//const std::string GREEN = "\033[38;5;2m";
//const std::string YELLOW = "\033[38;5;3m";
//const std::string RESET = "\033[0m";

//static std::string serverParameter[] = { "listen ", "host ", "server_name ",
//"error_page ", "client_max_body_size ", "root " , "index ", "allow_methods "};
//static std::string methods[] = { "GET", "POST", "DELETE", "PUT", "HEAD" };

template <typename T>
std::string toString(const T value)
{
	std::stringstream ss;
	ss << value;
	return ss.str();
}

#endif