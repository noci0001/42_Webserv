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
	std::string					        _port;
	std::string					        _server_host;
	std::string					        _server_name;
	std::string	    				    _error_page;
	std::string		    	    		_client_max_body_size;
	std::string			    	    	_root;
	std::string				        	_index;
protected:
public:
	int							        _status;
	Configfile();
	Configfile(Parsing *parser, std::string& config_file);
	std::string get_value_from_key(std::string keyword, int position);
	// int add_serverData(std::string keyword, std::string value);
	std::string obtain_serverdata(std::string keyword);
	void get_values_serverData();
	int find_all_server_keywords(std::string& config_file);
	int getTypePath(std::string const path);
	int	checkFile(std::string const path, int mode);
	~Configfile();
};

#endif
