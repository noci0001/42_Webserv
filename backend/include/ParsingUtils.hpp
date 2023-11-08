#ifndef PARSINGUTILS_HPP
# define PARSINGUTILS_HPP

#include "Webserv.hpp"

class ParsingUtils {
private:
public:
	ParsingUtils();
	static int message(std::string message, int ret);
	~ParsingUtils();
};

#endif