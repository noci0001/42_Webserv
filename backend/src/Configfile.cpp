#include "../include/Webserv.hpp"
#include "../include/Configfile.hpp"

Configfile::Configfile(): _file_size(0) {}

Configfile::Configfile(std::string const path) : _original_config_file(path), _file_size(0) {}

Configfile::~Configfile() {}

Configfile::Configfile(Parsing *parser, std::string& config_file): _original_config_file(config_file), _status(-1) {
	std::ifstream file("./configs/" + config_file);
	std::string line;

    if (parser->get_status() == FAILURE)
        this->_status = FAILURE;
	if (file.is_open()) {
		while (getline(file, line))
			this->_original_config_file += line + "\n";
		file.close();
		//this->_status = TBD;
	}
	else {
		this->_status = FAILURE;
		std::cout << "❌\t" << RED << "Unable to open file" << RESET << std::endl;
	}
	if (find_all_server_keywords(this->_original_config_file) == SUCCESS && parser->get_status() != FAILURE)
		std::cout << "✅\t" << GREEN << "All key-value pairs FOUND" << RESET << std::endl;
	else {
		this->_status = FAILURE;
		std::cout << RED << "❌\tCouldn't find all the keywords"<< RESET << std::endl;
        parser->set_status(FAILURE);
	}
    this->_status = SUCCESS;
}

Configfile::~Configfile() {
	if (this->_status == SUCCESS) {
		Configfile::get_values_serverData();
        std::cout << GREEN << "\n✅\tConfiguration correctly analyzed" << RESET << std::endl;
	}
	else {
		std::cout << RED << "❌\tError found in configuration file analysis" << RESET << std::endl;
    }
}

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