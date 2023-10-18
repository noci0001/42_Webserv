#include "../include/Webserv.hpp"

Configfile::Configfile(): _status(-1) {}

// Given a keyword string and its position in the config file
// the value is extracted by iterating through the string until
// the semicolon is found
std::string Configfile::get_value_from_key(std::string keyword, int position) {
	std::string value;
	std::size_t i = position;
	while (this->_original_config_file[i] != ';') {
		value += this->_original_config_file[i];
		i++;
	}
	this->_serverData[keyword] = value;
	return (value);
}

void Configfile::get_value_serverData() {
	std::cout << "KEY-VALUE PAIRS: \n" << std::endl;
    std::cout << std::left << std::setw(15) << BOLD << YELLOW << "KEY" << std::setw(25) << RESET << BOLD << YELLOW << "VALUE\n" << RESET << std::endl;
    for (std::map<std::string, std::string>::iterator it = _serverData.begin(); it != _serverData.end(); ++it) {
        std::cout << std::left << std::setw(25) << it->first << "|    " << std::setw(25) << it->second << std::endl;
    }
}

// Takes the config file as a string and checks that
// all the keywords are present. If they are, the value
// gets stored in a vector of strings
int Configfile::find_all_server_keywords(std::string& config_file) {
	std::cout << "\nBuilding key-value pairs \n" << std::endl;
	int i = 0;
	//const char *keywords[] = { "listen ", "host ", "server_name ", "error_page ", "client_max_body_size ", "root" , "index ", "allow_methods"};
    int key_found = 0;
	while (i < serverParameter->size()) {
		std::size_t position = config_file.find(serverParameter[i]);
		if (position != std::string::npos) {
            this->_serverData.insert(std::make_pair(serverParameter[i], this->get_value_from_key(serverParameter[i] , position += strlen(serverParameter[i].c_str()))));
			key_found++;
		}
		i++;
	}
	if (key_found == serverParameter->size()) {
		// this->get_value_serverData();
		return (SUCCESS);
	} else
		return (FAILURE);
}

Configfile::Configfile(Parsing *parser, std::string& config_file): _original_config_file(config_file), _status(-1) {
	std::ifstream file("./configs/" + config_file);
	std::string line;

    if (parser->get_status() == FAILURE)
        this->_status = FAILURE;
	if (file.is_open()) {
		while (getline(file, line))
			this->_original_config_file += line + "\n";
		file.close();
		this->_status = SUCCESS;
	}
	else {
		this->_status = FAILURE;
		std::cout << "❌\t" << RED << "Unable to open file" << RESET << std::endl;
	}
	if (find_all_server_keywords(this->_original_config_file) == SUCCESS && parser->get_status() != FAILURE)
		std::cout << "✅\t" << GREEN << "All key-value pairs FOUND" << RESET << std::endl;
	else {
		this->_status = FAILURE;
		std::cout << RED << "❌\tCouldn't find all the keywords"<< RESET << std::endl;
        parser->set_status(FAILURE);
	}
}

Configfile::~Configfile() {
	if (this->_status == SUCCESS) {
		Configfile::_status = SUCCESS;
		Configfile::get_value_serverData();
        std::cout << GREEN << "\n✅\tConfiguration correctly analyzed" << RESET << std::endl;
	}
	else {
		std::cout << RED << "❌\tError found in configuration file analysis" << RESET << std::endl;
    }
}