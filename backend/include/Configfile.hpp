#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include "ParsingUtils.hpp"

#include <fstream>
#include <map>
#include <string>

class Configfile: public ParsingUtils {
private:
	std::string					        _original_config_file;
	std::map<std::string, std::string>	_serverData;
protected:
public:
	int							        _status;
	Configfile();
	Configfile(Parsing *parser, std::string& config_file);
	std::string get_value_from_key(std::string keyword, int position);
	int serverData() { return (this->_status); };
    void get_value_serverData();
	int find_all_server_keywords(std::string& config_file);
	~Configfile();
};

#endif