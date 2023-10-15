#include "../include/Webserv.hpp"

Configfile::Configfile() {

}

Configfile::Configfile(std::string& config_file) {
	std::ifstream file("./configs/" + config_file);
	std::string line;
	if (file.is_open()) {
		while (getline(file, line)) {
			std::cout << line + "!!" << std::endl;
		}
		file.close();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}
}

Configfile::~Configfile() {

}