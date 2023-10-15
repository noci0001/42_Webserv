#include "../include/Webserv.hpp"

Parsing::Parsing() {
	if (config_file_exist() == 1)
		this->status = 1;
}

Parsing::Parsing(char *argv): config_file(argv), status(-1) {
	std::cout << "Parsing of config file" << std::endl;
	if (config_file_exist(config_file) == 1)
		this->status = 1;
}

// return 1 if config file doesn't exist
// return 0 if config file exist
int Parsing::config_file_exist(std::string& config_file) {
	std::ifstream file(config_file);

	if (file.is_open()) {
		std::cout << "Config file exist" << std::endl;
		file.close();
		return (0);
	} else {
		std::cout << "Config file doesn't exist" << std::endl;
		return (1);
	}
	return (0);
}

int Parsing::config_file_exist() {
	std::cout << "Config file with no args called" << std::endl;

	return (1);
}

Parsing::~Parsing() {
	if (this->status == 0)
		std::cout << "Parsing successful" << std::endl;
	else if (this->status == 1)
		std::cout << "ERROR: parsing unsuccessful" << std::endl;
}