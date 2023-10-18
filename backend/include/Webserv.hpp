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
#include <vector>
#include <iostream>
#include <string>
#include <cstring>
#include <utility>

const std::string BOLD = "\033[1m";
const std::string RED = "\033[38;5;1m";
const std::string GREEN = "\033[38;5;2m";
const std::string YELLOW = "\033[38;5;3m";
const std::string RESET = "\033[0m";

static std::string  serverParameter[] = { "listen ", "host ", "server_name ", "error_page ", "client_max_body_size ", "root" , "index ", "allow_methods"};

#endif