#ifndef CONFIGFILE_HPP
# define CONFIGFILE_HPP

#include "Webserv.hpp"

class Configfile
{
	private:
		std::string					        _original_config_file;
		size_t 								_file_size;

	public:
		Configfile();
		Configfile(std::string const config_file);
		~Configfile();

		static int getTypePath(std::string const path);
		static int checkFile(std::string const path, int mode);
		static int isFileExistAndReadable(std::string const path, std::string const index);
		std::string readFile(std::string path);

		std::string getOriginalConfigFile();
		int getFileSize();
};

#endif
