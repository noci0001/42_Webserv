#include "../../include/Webserv.hpp"

ParsingUtils::ParsingUtils() {}

ParsingUtils::~ParsingUtils() {}

// This function is used to print a message and RETURN an input value.
// REMINDER INPUT VALUE:
// 0: SUCCESS
// -1: (TBD) To Be Determined -> This status indicates that the
// 1: FAILURE
int ParsingUtils::message(std::string message, int ret) {
	std::cout << message << std::endl;
	return (ret);
}