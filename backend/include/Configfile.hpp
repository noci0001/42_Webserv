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
		size_t						        _file_size;

	public:
		Configfile();
		Configfile(std::string const path);
		~Configfile();
		
		static int getTypePath(std::string const path);
		static int checkFile(std::string const path, int mode);
		std::string readFile(std::string path);
		static int isFileExistAndReadable(std::string const path, std::string const index);

		std::string getOriginalConfigFile();
		int getFileSize();
};

#endif
