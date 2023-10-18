#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include "ParsingUtils.hpp"

#include <fstream>
#include <vector>



class Configfile: public ParsingUtils {
private:
	std::string					        _original_config_file;
	std::vector<std::string>            _serverData;
	std::string					        _port;
	std::string					        _server_host;
	std::string					        _server_name;
	std::string					        _error_page;
	std::string					        _client_max_body_size;
	std::string					        _root;
	std::string					        _index;
protected:
public:
	int							_status;
	Configfile();
	Configfile(std::string& config_file);
	std::string get_value_from_key(std::string keyword, std::size_t position);
	void get_value_serverData();
	int find_all_server_keywords(std::string& config_file);
	~Configfile();
};

#endif