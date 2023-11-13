#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include "Webserv.hpp"

#include <fstream>
#include <map>
#include <string>


class Configfile
{
	private:
		std::string					        _original_config_file;
<<<<<<< HEAD
		std::map<std::string, std::string>	_serverData;
=======
		size_t						        _file_size;
>>>>>>> de50599ac616b265271b3ce6b0c1ed598d2728c6

	protected:
	public:
		int							        _status;
		Configfile();
<<<<<<< HEAD
		Configfile(Parsing *parser, std::string& config_file);
		std::string get_value_from_key(std::string keyword, int position);
		// int add_serverData(std::string keyword, std::string value);
		std::string obtain_serverdata(std::string keyword);
		bool key_pairs_checking(std::map<std::string, std::string> serverData);
		void get_values_serverData();
		int find_all_server_keywords(std::string& config_file);
		static int getTypePath(std::string const path);
		static int checkFile(std::string const path, int mode);
		static int isFileExistAndReadable(std::string const path, std::string const index);
		~Configfile();
=======
		Configfile(std::string const path);
		~Configfile();
		
		static int getTypePath(std::string const path);
		static int checkFile(std::string const path, int mode);
		std::string readFile(std::string path);
		static int isFileExistAndReadable(std::string const path, std::string const index);

		std::string getOriginalConfigFile();
		int getFileSize();
>>>>>>> de50599ac616b265271b3ce6b0c1ed598d2728c6
};

#endif
