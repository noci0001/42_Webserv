#ifndef PARSING_HPP
# define PARSING_HPP

#include <fstream>
#include "Webserv.hpp"
#include "ParsingUtils.hpp"

class Parsing: public ParsingUtils {
private:
	std::string _config_file;
    int         _status;
protected:
public:
	Parsing();
	Parsing(char *argv);
	int config_file_exist(std::string& config_file);
	int config_file_exist();
	int document_parsing();
	int file_extension_check(std::string& config_file);
    int get_status() { return (_status);};
    void set_status(int status) { _status = status; };
	~Parsing();
};

#endif
