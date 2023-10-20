#include "../include/Webserv.hpp"

int main(int argc, char** argv) {
	std::string config_file_name;
	if (argc == 1) {
		std::cout << "Webserv starting in default mode" << std::endl;
		std::cout << "Default config file: " << "webserv.conf" << std::endl;
		Parsing parsing;
		if (parsing.get_status() == FAILURE) {
			return (Parsing::message("ERROR.\n Quitting...", FAILURE));
		}
		config_file_name = "webserv.conf";
		Configfile config_file(&parsing, config_file_name);
	} else if (argc == 2) {
		std::cout << "Webserv starting in custom mode" << std::endl;
		std::cout << "Config file: " << argv[1] << std::endl;
		Parsing parsing(argv[1]);
		config_file_name = argv[1];
		Configfile config_file(&parsing, config_file_name);
	}
	std::cout << "Webserv started" << std::endl;
	return (Parsing::message("Thanks for using our Webserv!", SUCCESS));
}