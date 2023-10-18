#include "../include/Webserv.hpp"

Configfile::Configfile(): _status(-1) {}

// Given a keyword string and its position in the config file
// the value is extracted by iterating through the string until
// the semicolon is found
std::string Configfile::get_value_from_key(std::string keyword, std::size_t position) {
	std::string value;
	std::size_t i = position;
	while (this->_original_config_file[i] != ';') {
		value += this->_original_config_file[i];
		i++;
	}
	this->_serverData.push_back(value);
	return (value);
}

void Configfile::get_value_serverData() {
    std::cout << "Currently getting all server data" << std::endl;
	for (int i = 0; i <= _serverData.size(); ++i) {
        std::cout << "Value from vector: " << this->_serverData[i] << std::endl;
    }
}

// Takes the config file as a string and checks that
// all the keywords are present. If they are, the value
// gets stored in a vector of strings
int Configfile::find_all_server_keywords(std::string& config_file) {
	std::cout << "Currently finding all server keywords\n" << std::endl;
	// std::cout << "CONFIG FILE IS:\n" << config_file << std::endl;
	int i = 0;
	const char *keywords[] = { "listen ", "host ", "server_name ", "error_page ", "client_max_body_size ", "root" , "index "};
	int key_found = 0;
	while (i < 6) {
		std::size_t position = config_file.find(keywords[i]);
		if (position != std::string::npos) {
			std::cout << "Keyword: " << (keywords[i]) << std::endl;
			this->get_value_from_key(keywords[i] , position += std::mbrlen(keywords[i], 30, NULL));
			std::cout << std::endl;
			key_found++;
		}
		i++;
	}
	std::cout << "Keys found: " << key_found << std::endl;
	if (key_found == 6) {
		// this->get_value_serverData();
		return (SUCCESS);
	} else
		return (FAILURE);
}

Configfile::Configfile(std::string& config_file): _original_config_file(config_file), _status(-1) {
	std::cout << "Configfile constructor called" << std::endl;
    std::ifstream file("./configs/" + config_file);
	std::string line;

	if (file.is_open()) {
		while (getline(file, line)) {
			this->_original_config_file += line + "\n";
			// Configfile::parseline(line);
		}
		file.close();
		this->_status = SUCCESS;
	}
	else {
		this->_status = FAILURE;
		std::cout << "Unable to open file" << std::endl;
	}
	if (find_all_server_keywords(this->_original_config_file) == SUCCESS) {
		std::cout << "All keywords FOUND" << std::endl;
        this->_status = SUCCESS;
    }
	else {
        this->_status = FAILURE;
        std::cout << "Couldn't find all the keywords" << std::endl;
    }
    Configfile::get_value_serverData();
}

Configfile::~Configfile() {
	if (this->_status == SUCCESS) {
		std::cout << "Configuration correctly analyzed" << std::endl;
	}
	else {
		std::cout << "Error found in configuration file analysis" << std::endl;
	}
}