#include "../include/Webserv.hpp"
#include <signal.h>

void	sigpipeHandle(int sig){	if (sig) {}}

int main (int argc, char **argv)
{
	if (argc == 1 || argc == 2) {
		try
		{
			std::string		config_file_name;
			ParsingUtils	configfile;
			ServerControler servercontroler;
			signal(SIGPIPE, sigpipeHandle);
			config_file_name = (argc == 1 ? "configs/webserv.conf" : argv[1]);
			configfile.createCluster(config_file_name);
			//configfile.printConfigFile();
			servercontroler.startServer(configfile.getServers());
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