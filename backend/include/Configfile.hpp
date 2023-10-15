#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include <fstream>

class Configfile {
private:
	int _status;
	std::string _port;
	std::string _server_host;
	std::string _server_name;
	std::string _error_page;
	std::string _client_max_body_size;
	std::string _root;
	std::string _index;
protected:
public:
	Configfile();
	Configfile(std::string& config_file);
	~Configfile();
};

#endif