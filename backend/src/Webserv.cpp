#include "../include/Webserv.hpp"

int main(int argc, char** argv) {
	std::string config_file_name;
    ParsingUtils utils;
    int webserv_can_start = FAILURE;
	if (argc == 1) {
		std::cout << BOLD << GREEN << "42FLIX Webserver STARTING IN DEFAULT MODE" << RESET << std::endl;
		std::cout << "Default config file: " << "webserv.conf\n" << std::endl;
		Parsing parsing;
		config_file_name = "webserv.conf";
		Configfile config_file(&parsing, config_file_name);
        config_file.get_value_serverData();
        if (config_file._status == SUCCESS) {
            std::cout << BOLD << GREEN << "✅\t\t42FLIX starting...\n" << RESET << std::endl;
            ServerConfig serverConf;
        } else
            utils.message("❌\t42FLIX failed to start: ", FAILURE);
	} else if (argc == 2) {
		std::cout << BOLD << GREEN << "42FLIX Webserver starting in custom mode\n" << RESET << std::endl;
		std::cout << BOLD << GREEN << "Config file: " << argv[1] << RESET << std::endl;
		Parsing parsing(argv[1]);
		config_file_name = argv[1];
		Configfile config_file(&parsing, config_file_name);
        config_file.get_value_serverData();
        if (config_file._status == SUCCESS) {
            std::cout << BOLD << GREEN << "✅\t\t42FLIX starting...\n" << RESET << std::endl;
                ServerConfig serverConf;
        } else
            utils.message("❌\t42FLIX failed to start: ", FAILURE);
    }
	return (Parsing::message("\t✨\tThanks for using our 42FLIX!\t✨", SUCCESS));
}