#include "../include/Webserv.hpp"

int main(int argc, char** argv) {
	if (argc == 1) {
		std::cout << "Webserv starting in default mode" << std::endl;
		std::cout << "Default config file: " << "webserv.conf" << std::endl;
		Parsing parsing;
		if (parsing.get_status() == 1) {
			std::cout << "ERROR.\n Quitting...";
			return (1);
		}
	}
	else if (argc == 2) {
		std::cout << "Webserv starting in custom mode" << std::endl;
		std::cout << "Config file: " << argv[1] << std::endl;
		Parsing parsing(argv[1]);
		if (parsing.get_status() == 1) {
			std::cout << "ERROR.\nQuitting...";
			return (1);
		}
	}
	else {
		std::cout << "ERROR.\nQuitting..." ;
		return (1);
	}
	std::cout << "Webserv started" << std::endl;
	return (0);
}