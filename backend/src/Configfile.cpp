#include "../include/Webserv.hpp"
#include "../include/Configfile.hpp"

Configfile::Configfile(): _file_size(0) {}

Configfile::Configfile(std::string const path) : _original_config_file(path), _file_size(0) {}

Configfile::~Configfile() {}

//[* defines if path is: file(1), directory(2) or other(3) *]
int Configfile::getTypePath(std::string const path)
{
	struct stat buffer;
	int			result;

	result = stat(path.c_str(), &buffer);
	if (result == 0)
	{
		if (buffer.st_mode & S_IFREG)
			return 1;
		else if (buffer.st_mode & S_IFDIR)
			return 2;
		else
			return 3;
	}
	else
		return -1;
}

//[* checks if file is accessible to client *]
int Configfile::checkFile(std::string const path, int mode)
{
	return (access(path.c_str(), mode));
}

int Configfile::isFileExistAndReadable(std::string const path, std::string const index)
{
	if (getTypePath(index) == 1 && checkFile(index, 4) == 0)
		return (0);
	if (getTypePath(path + index) == 1 && checkFile(path + index, 4) == 0)
		return (0);
	return (-1);
}

std::string Configfile::readFile(std::string path)
{
	if (path.empty() || path.length() == 0)
		return NULL;
	std::ifstream config_file(path.c_str());
	if (!config_file || !config_file.is_open())
		return NULL;
	std::stringstream ss;
	ss << config_file.rdbuf();
	return ss.str();
}

std::string Configfile::getOriginalConfigFile()
{
	return this->_original_config_file;
}

int Configfile::getFileSize()
{
	return this->_file_size;
}