#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include "Webserv.hpp"
#include "Parsing.hpp"
#include "ParsingUtils.hpp"
#include "ServerConfig.hpp"
#include "Location.hpp"
#include <map>

class Configfile
{
	private:
		std::string					        _original_config_file;
		std::map<std::string, std::string>	_serverData;
		size_t 								_file_size;

	protected:
	public:
		int							        _status;
		Configfile();
		Configfile(std::string const config_file);
		~Configfile();

		std::string get_value_from_key(std::string keyword, int position);
		// int add_serverData(std::string keyword, std::string value);
		std::string obtain_serverdata(std::string keyword);
		bool key_pairs_checking(std::map<std::string, std::string> serverData);
		void get_values_serverData();
		int find_all_server_keywords(std::string& config_file);
		static int getTypePath(std::string const path);
		static int checkFile(std::string const path, int mode);
		static int isFileExistAndReadable(std::string const path, std::string const index);
		std::string readFile(std::string path);

		std::string getOriginalConfigFile();
		int getFileSize();
};

#endif
