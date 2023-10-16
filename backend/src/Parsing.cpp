#include "../include/Webserv.hpp"

//START DEFAULT CONSTRUCTOR
Parsing::Parsing(): _config_file("webserv.conf"), _status(TBD){
	std::cout << "Parsing of config file" << std::endl;
	config_file_exist();
	if (this->_status == TBD && file_extension_check(this->_config_file) == 0) {
		std::cout << "Parsing: file extension is correct" << std::endl;
		this->_status = SUCCESS;
	} else {
		std::cout << "Parsing: file extension is incorrect" << std::endl;
		this->_status = FAILURE;
	}
}

int Parsing::config_file_exist() {
	std::ifstream file("./configs/" + this->_config_file);
	if (file.is_open()) {
		std::cout << "Config file exist" << std::endl;
		file.close();
		return (SUCCESS);
	} else {
		std::cout << "Config file doesn't exist" << std::endl;
		this->_status = FAILURE;
		return (FAILURE);
	}
	return (SUCCESS);
}
//END DEFAULT CONSTRUCTOR


//START CUSTOM CONSTRUCTOR
Parsing::Parsing(char *argv): _config_file(argv), _status(TBD) {
	std::cout << "Parsing of custom config file" << std::endl;
	config_file_exist(_config_file);
	if (this->_status == TBD && file_extension_check(_config_file) == 0) {
		std::cout << "Parsing: file extension is correct" << std::endl;
		this->_status = SUCCESS;
	} else {
		std::cout << "Parsing: file extension is incorrect" << std::endl;
		this->_status = FAILURE;
	}
}

// return 1 if config file doesn't exist or error
// return 0 if config file exist
int Parsing::config_file_exist(std::string& config_file) {
	if (config_file == "./configs/webserv.conf") {
		std::cout << "Default config file selected in custom mode" << std::endl;
		return (SUCCESS);
	}
	std::ifstream file("./configs/" + config_file);

	if (file.is_open()) {
		std::cout << "Config file exist" << std::endl;
		file.close();
		return (SUCCESS);
	} else {
		std::cout << "Config file doesn't exist!" << std::endl;
		this->_status = FAILURE;
		return (FAILURE);
	}
	return (SUCCESS);
}

int Parsing::file_extension_check(std::string& config_file) {
	std::string extension = ".conf";
	std::string::size_type idx = config_file.rfind('.');
	if (idx != std::string::npos) {
		std::string file_extension = config_file.substr(idx);
		if (file_extension == extension) {
			return (SUCCESS);
		}
		else {
			std::cout << "Parsing: file extension is incorrect" << std::endl;
			this->_status = FAILURE;
			return (FAILURE);
		}
	}
	else {
		std::cout << "Parsing: file extension is incorrect" << std::endl;
		this->_status = FAILURE;
		return (FAILURE);
	}
	return (SUCCESS);
}
//END CUSTOM CONSTRUCTOR


//PARSING OF DOCUMENT (COMMON FOR BOTH CONSTRUCTORS)
int Parsing::document_parsing() {
	Configfile config_file(this->_config_file);
	if (config_file._status == SUCCESS)
		return (SUCCESS);
	else
		return (FAILURE);
}

//DESTRUCTOR
Parsing::~Parsing() {
	if (this->_status == SUCCESS){
		std::cout << "First parsing successful" << std::endl;
	} else if (this->_status == FAILURE)
		std::cout << "Parsing unsuccessful" << std::endl;
}