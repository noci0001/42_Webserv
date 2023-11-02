#include "../include/Webserv.hpp"

Configfile::Configfile(): _status(-1) {}

bool Configfile::key_pairs_checking(std::map<std::string, std::string> serverData) {
    std::cout << "Key value pairs CHECKING\n" << std::endl;

    //LISTENING PORT
    std::string int_version_of_value = serverData["listen "];
    std::istringstream ss(int_version_of_value);
    int int_value;
    if (ss >> int_value) {
        if (int_value >= 1024 && int_value <= 65353)
            std::cout << "✅\t" << "Listening port value correctly set to " << int_value << std::endl;
        else {
            std::cout << "❌\t" << "Listening port value out of range: must be between 1024 up to 65353" << std::endl;
            this->_status = FAILURE;
            return (false);
        }
    }

    //HOST
    if (serverData["host "] == "127.0.0.1")
        std::cout << "✅\t" << "Host correctly set to loopback address of " << serverData["host "] << std::endl;
    else {
        std::cout << "❌\t" << "Host incorrectly set to " << serverData["host "] << std::endl;
        this->_status = FAILURE;
        return (false);
    }

    //SERVER NAME
    if (serverData["server_name "] == "localhost")
        std::cout << "✅\t" << "Server Name correctly set to localhost" << std::endl;
    else {
        std::cout << "❌\t" << "Server Name incorrectly set to localhost" << std::endl;
        this->_status = FAILURE;
        return (false);
    }

    //ERROR PAGE
    if (serverData["error_page "].find(' ') != std::string::npos) {
        std::istringstream ss(serverData["error_page "]);
        std::vector<std::string> tokens;
        std::string token;
        while (ss >> token)
            tokens.push_back(token);

        std::string error_path =  "/error_pages/" + tokens[0] + ".html";
        // 4xx error_pages/4xx.html
        if (tokens[0][0] != '4' || (tokens[1].compare(error_path) != 0 && tokens[1].compare("/root/" + tokens[0] + ".html") != 0)) {
            std::cout << "❌\t"
                      << "Error page incorrectly set. Error present in either error code (not in the range 400 - 499)."
                      << std::endl << "ERROR CODE in configuration file: " << tokens[0] << std::endl
                      << "ERROR PATH in configuration file: " << tokens[1] << std::endl;
            this->_status = FAILURE;
            return (false);
        }
        else
            std::cout << "✅\t" << "Error page correctly set to " << serverData["error_page "] << std::endl;
    }
    else {
        std::cout << "❌\t" << "Error page incorrectly set" << std::endl;
        this->_status = FAILURE;
        return (false);
    }

    //CLIENT MAX BODY SIZE
    int_version_of_value = serverData["client_max_body_size "];
    int_value = 0;
    //CMBS -> short for CLIENT MAX BODY SIZE
    std::istringstream cmbs_ss(int_version_of_value);
    if (cmbs_ss >> int_value) {
        if (int_value <= MAX_CONTENT_LENGTH)
            std::cout << "✅\t" << "Client max body size correctly set to " << int_value << std::endl;
        else {
            std::cout << "❌\t" << "Client max body size set to out of range value" << std::endl;
            this->_status = FAILURE;
            return (false);
        }
    } else {
        std::cout << "❌\t" << "Operation failed at Client Max Body Size " << std::endl;
        this->_status = FAILURE;
        return (false);
    }

    //ROOT
    if (serverData["root "] == "/root/index.html" || serverData["root"] == "root/index.html")
        std::cout << "✅\t" << "Root correctly set to " << serverData["root "] << std::endl;
    else {
        std::cout << "❌\t" << "Root incorrectly set to "<< serverData["root "] << std::endl;
        this->_status = FAILURE;
        return (false);
    }

    //INDEX
    if (serverData["index "] == "/root/index.html" || serverData["index "] == "root/index.html")
        std::cout << "✅\t" << "Index correctly set to path " << serverData["index "] << std::endl;
    else {
        std::cout << "❌\t" << "Index incorrectly set to " << serverData["index"] << std::endl;
        this->_status = FAILURE;
        return (false);
    }

    //ALLOW METHODS
    std::istringstream allow_methods(serverData["allow_methods "]);
    std::vector<std::string> tokens;
    std::string token;
    int methods_found = 0;
    while (allow_methods >> token) {
        int possible_total_methods = methods->size();
        while (possible_total_methods != -1) {
            if (token == methods[possible_total_methods]) {
                std::cout << "✅\t" << "FOUND METHOD: " << methods[possible_total_methods] << std::endl;
                methods_found++;
            }
            possible_total_methods--;
        }
        tokens.push_back(token);
    }
    if (methods_found == 0) {
        std::cout << "❌\t" << "Allow methods not set!" << std::endl;
        this->_status = FAILURE;
        return (false);
    } else if (methods_found != tokens.size()) {
        std::cout << "❌\t" << "Allow methods incorrectly set" << std::endl;
        this->_status = FAILURE;
        return (false);
    } else
        std::cout << "✅\t" << "Allow methods correctly set" << std::endl;

    this->_status = SUCCESS;
    std::cout << std::endl;
    return (true);
}

// Given a keyword string and its position in the config file
// the value is extracted by iterating through the string until
// the semicolon is found
std::string Configfile::get_value_from_key(std::string keyword, int position) {
	std::string value;
	std::size_t i = position;
	while (this->_original_config_file[i] != ';') {
		value += this->_original_config_file[i];
		i++;
	}
    this->_serverData[keyword] = value;
    return (value);
}

std::string Configfile::obtain_serverdata(std::string keyword) {
    std::cout << "RETURN VALUE: " << this->_serverData[keyword] << std::endl;
    return this->_serverData[keyword];
}

void Configfile::get_values_serverData() {
	std::cout << "Key-value pairs: \n" << std::endl;
    for (int i = 0; i < 65; i++)
        std::cout << "-";
    std::cout << std::endl;
    std::cout << std::left << std::setw(15) << BOLD << YELLOW << "KEY" << std::setw(25) << RESET << BOLD << YELLOW << "VALUE\n" << RESET<< std::endl;
    for (std::map<std::string, std::string>::iterator it = _serverData.begin(); it != _serverData.end(); ++it) {
        std::cout << "|    " << std::left << std::setw(25) << it->first << "|    " << std::setw(25) << it->second << "    |"<< std::endl;
    }
    for (int i = 0; i < 65; i++)
        std::cout << "-";
}

// Takes the config file as a string and checks that
// all the keywords are present. If they are, the value
// gets stored in a vector of strings
int Configfile::find_all_server_keywords(std::string& config_file) {
	std::cout << "\nBuilding key-value pairs \n" << std::endl;
	int i = 0;
	//const char *keywords[] = { "listen ", "host ", "server_name ", "error_page ", "client_max_body_size ", "root" , "index ", "allow_methods"};
    int key_found = 0;
    int parameter_array_size = serverParameter->size();
    parameter_array_size += 1;
	while (i < parameter_array_size) {
		std::size_t position = config_file.find(serverParameter[i]);
		if (position != std::string::npos) {
            this->_serverData.insert(std::make_pair(serverParameter[i], this->get_value_from_key(serverParameter[i] , position += strlen(serverParameter[i].c_str()))));
			key_found++;
		}
		i++;
	}
    bool checking_ret = this->key_pairs_checking(_serverData);
	if (key_found == parameter_array_size) {
        if (checking_ret)
		    return (SUCCESS);
        else
            return (FAILURE);
	} else
		return (FAILURE);
}

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
		else if (buffer. st_mode & S_IFDIR)
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