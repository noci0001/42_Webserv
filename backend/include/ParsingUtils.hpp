#ifndef PARSINGUTILS_HPP
# define PARSINGUTILS_HPP

#include "Webserv.hpp"
#include "ServerConfig.hpp"
#include "Configfile.hpp"

class ServerConfig;

class ParsingUtils
{
	private:
		std::vector<ServerConfig> _server_config;
		std::vector<std::string>  _config_file;
		size_t 					  _server_number;

	public:
		ParsingUtils();
		~ParsingUtils();

		int createCluster(const std::string &config_file);

		void splitConfigFile(std::string &content);
		void removeComments(std::string &content);
		void removeWhiteSpaces(std::string &content);
		size_t findServerStart(size_t start, std::string &content);
		size_t findServerEnd(size_t start, std::string &content);
		void createServer(std::string &config_file, ServerConfig &server_config);
		void checkServers();
		std::vector<ServerConfig> getServers();
		int strCompare(std::string str1, std::string str2, size_t position);

		int printConfigFile();

	public:
		class ErrorException : public std::exception
		{
			private:
				std::string _message;
			public:
				ErrorException(std::string message) throw()
				{
					_message = "ParsingUtils: Error: " + message;
				}
				virtual const char *what() const throw()
				{
					return (_message.c_str());
				}
				virtual ~ErrorException() throw() {}
		};
};

#endif