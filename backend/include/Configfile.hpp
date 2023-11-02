#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include "Webserv.hpp"

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
	// int add_serverData(std::string keyword, std::string value);
	std::string obtain_serverdata(std::string keyword);
    bool key_pairs_checking(std::map<std::string, std::string> serverData);
	void get_values_serverData();
	int find_all_server_keywords(std::string& config_file);
	int getTypePath(std::string const path);
	int	checkFile(std::string const path, int mode);
	int isFileExistAndReadable(std::string const path, std::string const index);
	~Configfile();
};

#endif
