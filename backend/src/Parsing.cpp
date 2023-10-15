#include "../include/Webserv.hpp"

//START DEFAULT CONSTRUCTOR
Parsing::Parsing(): config_file("webserv.conf"), status(-1){
	std::cout << "Parsing of config file" << std::endl;
	config_file_exist();
	if (this->status == -1 && file_extension_check(this->config_file) == 0) {
		std::cout << "Parsing: file extension is correct" << std::endl;
		this->document_parsing();
	}
}

int Parsing::config_file_exist() {
	std::ifstream file("./configs/" + this->config_file);
	if (file.is_open()) {
		std::cout << "Config file exist" << std::endl;
		file.close();
		return (0);
	} else {
		std::cout << "Config file doesn't exist" << std::endl;
		this->status = 1;
		return (1);
	}
	return (0);
}

//END DEFAULT CONSTRUCTOR

//START CUSTOM CONSTRUCTOR
Parsing::Parsing(char *argv): config_file(argv), status(-1) {
	std::cout << "Parsing of custom config file" << std::endl;
	config_file_exist(config_file);
	if (this->status == -1 && file_extension_check(config_file) == 0) {
		std::cout << "Parsing: file extension is correct" << std::endl;
		this->document_parsing();
	}
}

// return 1 if config file doesn't exist or error
// return 0 if config file exist
int Parsing::config_file_exist(std::string& config_file) {
	if (config_file == "./configs/webserv.conf") {
		std::cout << "Default config file selected in custom mode" << std::endl;
		return (0);
	}
	std::ifstream file("./configs/" + config_file);

	if (file.is_open()) {
		std::cout << "Config file exist" << std::endl;
		file.close();
		return (0);
	} else {
		std::cout << "Config file doesn't exist!" << std::endl;
		this->status = 1;
		return (1);
	}
	return (0);
}

int Parsing::file_extension_check(std::string& config_file) {
	std::string extension = ".conf";
	std::string::size_type idx = config_file.rfind('.');
	if (idx != std::string::npos) {
		std::string file_extension = config_file.substr(idx);
		if (file_extension == extension) {
			std::cout << "Parsing: file extension is correct" << std::endl;
			return (0);
		}
		else {
			std::cout << "Parsing: file extension is incorrect" << std::endl;
			this->status = 1;
			return (1);
		}
	}
	else {
		std::cout << "Parsing: file extension is incorrect" << std::endl;
		this->status = 1;
		return (1);
	}
	return (0);
}

//END CUSTOM CONSTRUCTOR

//PARSING OF DOCUMENT (COMMON FOR BOTH CONSTRUCTORS)
int Parsing::document_parsing() {
	Configfile config_file(this->config_file);
}

//DESTRUCTOR
Parsing::~Parsing() {
	if (this->status == 0)
		std::cout << "Parsing successful" << std::endl;
	else if (this->status == 1)
		std::cout << " parsing unsuccessful" << std::endl;
}