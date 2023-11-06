#include "../include/Webserv.hpp"

/*int main(int argc, char** argv)
{
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
}*/

void	sigpipeHandler(int sig)
{
	if (sig) {}
}

int main (int argc, char **argv)
{
	if (argc == 1 || argc == 2) {
		try {
			std::string config_file_name;
			Configfile	configfile;
			ServerControler servercontroler;
			signal(SIGPIPE, sigpipeHandler);
			config_file_name = (argc == 1 ? "configs/webserv.conf" : argv[1]);
			configfile.get_values_serverData();
			servercontroler.startServers(configfile);
			servercontroler.runServers();
		}
		catch (std::exception &e) {
			std::cerr << e.what() << std::endl;
			return 1;
		}
	}
	else
	{
		ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "Error: Wrong number of arguments");
		return 1;
	}
	return 0;
}