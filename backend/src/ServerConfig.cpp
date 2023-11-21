#include "../include/Webserv.hpp"
#include "../include/ServerConfig.hpp"
#include <vector>

ServerConfig::ServerConfig()
{
    this->_port = 0;
    this->_host = 0;
    this->_server_name = "";
    this->_root = "";
    this->_max_body_size_client = MAX_CONTENT_LENGTH;
    this->_index = "";
    this->_fd_listen = 0;
    this->_autoindex = false;
    this->initErrorPages();
}

ServerConfig::~ServerConfig()
{}

//[**** Copy Constructor ****]
ServerConfig::ServerConfig(const ServerConfig &other)
{
    if (this != &other)
    {
        this->_server_name = other._server_name;
        this->_root = other._root;
        this->_host = other._host;
        this->_port = other._port;
        this->_max_body_size_client = other._max_body_size_client;
        this->_index = other._index;
        this->_error_pages = other._error_pages;
        this->_locations = other._locations;
        this->_fd_listen = other._fd_listen;
        this->_autoindex = other._autoindex;
        this->_server_address = other._server_address;
    }
	return ;
}

//[**** Assignment Operator ****]

ServerConfig &ServerConfig::operator=( const ServerConfig &rhs)
{
    if (this != &rhs)
    {
        this->_server_name = rhs._server_name;
        this->_root = rhs._root;
        this->_host = rhs._host;
        this->_port = rhs._port;
        this->_max_body_size_client = rhs._max_body_size_client;
        this->_index = rhs._index;
        this->_error_pages = rhs._error_pages;
        this->_locations = rhs._locations;
        this->_fd_listen = rhs._fd_listen;
        this->_autoindex = rhs._autoindex;
        this->_server_address = rhs._server_address;
    }
    return (*this);
}

//[* init error pages by default *]

void    ServerConfig::initErrorPages( void )
{
    _error_pages[301] = "";
    _error_pages[302] = "";
    _error_pages[400] = "";
    _error_pages[401] = "";
    _error_pages[402] = "";
    _error_pages[403] = "";
    _error_pages[404] = "";
    _error_pages[405] = "";
    _error_pages[406] = "";
    _error_pages[500] = "";
    _error_pages[501] = "";
    _error_pages[502] = "";
    _error_pages[503] = "";
	_error_pages[504] = "";
    _error_pages[505] = "";
}

//[**** Set Functions ****]

void    ServerConfig::setServerName(std::string server_name)
{
    checkToken(server_name);
    this->_server_name = server_name;
}

void    ServerConfig::setHost(std::string parameter)
{
    checkToken(parameter);
    if (parameter == "localhost")
        parameter = "127.0.0.1";
    if (!validHost(parameter))
        throw ErrorException("Wrong syntax: host");
    this->_host = inet_addr(parameter.data());
}

void    ServerConfig::setRoot(std::string root)
{
    checkToken(root);
    if (Configfile::getTypePath(root) == 2)
    {
        this->_root = root;
        return ;
    }
    char    directory[1024];
    getcwd(directory, 1024);
    std::string full_root = directory + root;
    if (Configfile::getTypePath(full_root) != 2)
        throw ErrorException("Wrong Syntax: root");
    this->_root = full_root;
}

void    ServerConfig::setPorts(std::string parameter)
{
    unsigned int    port;

    port = 0;
    checkToken(parameter);
    for (size_t i = 0; i < parameter.length(); i++)
    {
        if (!std::isdigit(parameter[i]))
            throw ErrorException("Wrong Syntax: port");
    }
    port = ft_stoi((parameter));
    if (port < 1 || port > 65636)
        throw ErrorException("Wrong Syntax: port");
    this->_port = (uint16_t) port;
}

void    ServerConfig::setMaxBodySizeClient(std::string parameter)
{
    unsigned long   body_size;

    body_size = 0;
    checkToken(parameter);
    for (size_t i = 0; i < parameter.length(); i++)
    {
        if (parameter[i] < '0' || parameter[i] > '9')
            throw ErrorException("Wrong Syntax: max_body_size_client");
    }
    if (!ft_stoi(parameter))
        throw ErrorException("Wrong Syntax: max_body_size_client");
    body_size = ft_stoi(parameter);
    this->_max_body_size_client = body_size;
}

void    ServerConfig::setIndex(std::string index)
{
    checkToken(index);
    this->_index = index;
}

void    ServerConfig::setAutoIndex(std::string autoindex)
{
    checkToken(autoindex);
    if (autoindex != "on" && autoindex != "off")
        throw ErrorException("Wrong syntax: autoindex");
    if (autoindex == "on")
        this->_autoindex = true;
}

//[* checks if there already is a standard error code. If found, overwrites path to file,
//  otherwise creates a new pair: error code + path_to_file *]
void    ServerConfig::setErrorPages(std::vector<std::string> &parameter)
{
    if (parameter.empty())
        return ;
    if (parameter.size() % 2 != 0)
        throw ErrorException("Error Page init failed");
    for (size_t i = 0; i < parameter.size() - 1; i++)
    {
        for (size_t j = 0; j < parameter[i].size(); j++)
        {
            if (!std::isdigit(parameter[i][j]))
                throw ErrorException("Error Code is invalid");
        }
        if (parameter[i].size() != 3)
            throw ErrorException("Error Code invalid");
        short error_code = ft_stoi(parameter[i]);
        if (statusCodeString(error_code) == "Undefined" || error_code < 400)
            throw ErrorException("Invalid Error Code" + parameter[i]);
        i++;
        std::string path = parameter[i];
        checkToken(path);
        if (Configfile::getTypePath(path) != 2)
        {
            if (Configfile::getTypePath(this->_root + path) != 1)
                throw ErrorException("Incorrect Path for Error Page file: " + this->_root + path);
            if (Configfile::checkFile(this->_root + path, 0) == -1 || Configfile::checkFile(this->_root + path, 4) == -1)
                throw ErrorException("Error Page file:" + this->_root + path + " is not accessible");
        }
        std::map<short, std::string>::iterator ito = this->_error_pages.find(error_code);
        if (ito != _error_pages.end())
            this->_error_pages[error_code] = path;
        else
            this->_error_pages.insert(std::make_pair(error_code, path));
    }
}

void    ServerConfig::setLocations(std::string path, std::vector<std::string> parameter)
{
	Location location_new;
	std::vector<std::string> methods;
	bool flag_methods = false;
	bool flag_autoindex = false;
	bool flag_max_size = false;
	int valid;

	location_new.setPath(path);
	for (size_t i = 0; i < parameter.size(); i++)
	{
		if (parameter[i] == "root" && (i + 1) < parameter.size())
		{
			if (!location_new.getRoot().empty())
				throw ErrorException("setLocation: root already set");
			checkToken(parameter[++i]);
			if (Configfile::getTypePath(parameter[i]) == 2)
				location_new.setRoot(parameter[i]);
			else
				location_new.setRoot(this->_root + parameter[i]);
		}
		else if ((parameter[i] == "allow_methods" || parameter[i] == "methods") && (i + 1) < parameter.size())
		{
			if (flag_methods)
				throw ErrorException("setLocation: methods already set");
			std::vector<std::string> methods;
			while (++i < parameter.size())
			{
				if (parameter[i].find(";") != std::string::npos)
				{
					checkToken(parameter[i]);
					methods.push_back(parameter[i]);
					break ;
				}
				else
				{
					methods.push_back(parameter[i]);
					if (i + 1 >= parameter.size())
						throw ErrorException("setLocation: methods not closed");
				}
			}
			location_new.setMethods(methods);
			flag_methods = true;
		}
		else if (parameter[i] == "autoindex" && (i + 1) < parameter.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("setLocation: autoindex not allowed in cgi-bin");
			if (flag_autoindex)
				throw ErrorException("setLocation: autoindex already set");
			checkToken(parameter[++i]);
			location_new.setAutoIndex(parameter[i]);
			flag_autoindex = true;
		}
		else if (parameter[i] == "index" && (i + 1) < parameter.size())
		{
			if (!location_new.getIndex().empty())
				throw ErrorException("setLocation: index already set");
			checkToken(parameter[++i]);
			location_new.setIndex(parameter[i]);
		}
		else if (parameter[i] == "return" && (i + 1) < parameter.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("setLocation: return not allowed in cgi-bin");
			if (!location_new.getReturn().empty())
				throw ErrorException("setLocation: return already set");
			checkToken(parameter[++i]);
			location_new.setReturn(parameter[i]);
		}
		else if (parameter[i] == "alias" && (i + 1) < parameter.size())
		{
			if (path == "/cgi-bin")
				throw ErrorException("setLocation: alias not allowed in cgi-bin");
			if (!location_new.getAlias().empty())
				throw ErrorException("setLocation: alias already set");
			checkToken(parameter[++i]);
			location_new.setAlias(parameter[i]);
		}
		else if (parameter[i] == "cgi_ext" && (i + 1) < parameter.size())
		{
			std::vector<std::string> cgi_extension;
			while (++i < parameter.size())
			{
				if (parameter[i].find(";") != std::string::npos)
				{
					checkToken(parameter[i]);
					cgi_extension.push_back(parameter[i]);
					break ;
				}
				else
				{
					cgi_extension.push_back(parameter[i]);
					if (i + 1 >= parameter.size())
						throw ErrorException("setLocation: cgi_ext not closed");
				}
			}
			location_new.setCgiExtension(cgi_extension);
		}
		else if (parameter[i] == "cgi_path" && (i + 1) < parameter.size())
		{
			std::vector<std::string> cgi_path;
			while (++i < parameter.size())
			{
				if (parameter[i].find(";") != std::string::npos)
				{
					checkToken(parameter[i]);
					cgi_path.push_back(parameter[i]);
					break ;
				}
				else
				{
					cgi_path.push_back(parameter[i]);
					if (i + 1 >= parameter.size())
						throw ErrorException("setLocation: cgi_path not closed");
				}
				if (parameter[i].find("/python") == std::string::npos && parameter[i].find("/bash") == std::string::npos)
					throw ErrorException("setLocation: cgi_path not valid");
			}
			location_new.setCgiPath(cgi_path);
		}
		else if (parameter[i] == "client_max_body_size" && (i + 1) < parameter.size())
		{
			if (flag_max_size)
				throw ErrorException("setLocation: client_max_body_size already set");
			checkToken(parameter[++i]);
			location_new.setMaxBodySizeClient(parameter[i]);
			flag_max_size = true;
		}
		else if (i < parameter.size())
			throw ErrorException("setLocation: invalid parameter");
	}
	if (location_new.getPath() != "/cgi-bin" && location_new.getIndex().empty())
		location_new.setIndex(this->_index);
	if (!flag_max_size)
		location_new.setMaxBodySizeClient(this->_max_body_size_client);
	valid = isValidLocation(location_new);
	if (valid == 1)
		throw ErrorException("CGI validation failed");
	else if (valid == 2)
		throw ErrorException("Path in location validation failed");
	else if (valid == 3)
		throw ErrorException("Redirection file validation failed");
	else if (valid == 4)
		throw ErrorException("Alias validation failed");
	this->_locations.push_back(location_new);
}

void    ServerConfig::setFdListen(int fd_listen)
{
    this->_fd_listen = fd_listen;
}

bool    ServerConfig::validHost(std::string host) const
{
    struct sockaddr_in sockaddr;
    return (inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) ? true : false);
}

bool	ServerConfig::validErrorPages()
{
	std::map<short, std::string>::const_iterator cito;
	for (cito = this->_error_pages.begin(); cito != this->_error_pages.end(); cito++)
	{
		if (cito->first < 100 || cito->first > 599)
			return (false);
		if (Configfile::checkFile(getRoot() + cito->second, 0) < 0
			|| Configfile::checkFile(getRoot() + cito->second, 4) < 0)
			return (false);
	}
	return (true);
}

int 	ServerConfig::isValidLocation(Location &location) const
{
	if (location.getPath() == "/cgi-bin")
	{
		if (location.getCgiPath().empty() || location.getCgiExtension().empty() || location.getIndex().empty())
			return (1);
		if (Configfile::checkFile(location.getIndex(), 4) < 0)
		{
			std::string path = location.getRoot() + location.getPath() + "/" + location.getIndex();
			if (Configfile::getTypePath(path) != 1)
			{
				std::string root = getcwd(NULL, 0);
				location.setRoot(root);
				path = location.getPath() + "/" + location.getIndex();
			}
			if (path.empty() || Configfile::getTypePath(path) != 1 || Configfile::checkFile(path, 4) < 0)
				return (1);
		}
		if (location.getCgiPath().size() != location.getCgiExtension().size())
			return (1);
		std::vector<std::string>::const_iterator cito;
		for (cito = location.getCgiPath().begin(); cito != location.getCgiPath().end(); ++cito)
		{
			if (Configfile::getTypePath(*cito) < 0)
				return (1);
		}
		std::vector<std::string>::const_iterator cito_path;
		for (cito = location.getCgiExtension().begin(); cito != location.getCgiExtension().end(); ++cito)
		{
			std::string temp = *cito;
			if (temp != ".py" && temp != ".sh" && temp != "*.py" && temp != "*.sh")
				return (1);
			for (cito_path = location.getCgiPath().begin(); cito_path != location.getCgiPath().end(); ++cito_path)
			{
				std::string temp_path = *cito_path;
				if (temp == ".py" || temp == "*.py")
				{
					if (temp_path.find("python3") != std::string::npos)
						location._extension_path.insert(std::make_pair(".py", temp_path));
				}
				else if (temp == ".sh" || temp == "*.sh")
				{
					if (temp_path.find("bash") != std::string::npos)
						location._extension_path[".sh"] = temp_path;
				}
			}
		}
		if (location.getCgiPath().size() != location.getExtensionPath().size())
			return (1);
	}
	else
	{
		if (location.getPath()[0] != '/')
			return (2);
		if (location.getRoot().empty())
			location.setRoot(this->_root);
		if (Configfile::isFileExistAndReadable(location.getRoot() + location.getPath() + "/", location.getIndex()))
			return (5);
		if (!location.getReturn().empty())
		{
			if (Configfile::isFileExistAndReadable(location.getRoot(), location.getReturn()))
				return (3);
		}
		if (!location.getAlias().empty())
		{
			if (Configfile::isFileExistAndReadable(location.getRoot(), location.getAlias()))
				return (4);
		}
	}
	return (0);
}

void	ServerConfig::checkToken(std::string &parameter)
{
	size_t pos = parameter.rfind(";");
	if (pos != parameter.size() - 1)
		throw ErrorException("checkToken: invalid token");
	parameter.erase(pos);
}

bool	ServerConfig::checkLocation() const
{
	if (this->_locations.size() < 2)
		return (false);
	std::vector<Location>::const_iterator cito1;
	std::vector<Location>::const_iterator cito2;
	for (cito1 = this->_locations.begin(); cito1 != this->_locations.end(); cito1++)
	{
		for (cito2 = cito1 + 1; cito2 != this->_locations.end(); cito2++)
		{
			if (cito1->getPath() == cito2->getPath())
				return (true);
		}
	}
	return (false);
}

//[**** Getter Functions ****]

const std::string &ServerConfig::getServerName()
{
    return this->_server_name;
}

const std::string &ServerConfig::getRoot()
{
    return this->_root;
}

const bool &ServerConfig::getAutoIndex()
{
    return this->_autoindex;
}

const in_addr_t &ServerConfig::getHost()
{
    return this->_host;
}

const uint16_t &ServerConfig::getPort()
{
    return this->_port;
}

const size_t &ServerConfig::getMaxBodySizeClient()
{
    return this->_max_body_size_client;
}

const std::string &ServerConfig::getIndex()
{
    return this->_index;
}

const std::map<short, std::string> &ServerConfig::getErrorPages()
{
    return this->_error_pages;
}

const std::vector<Location> &ServerConfig::getLocations()
{
    return this->_locations;
}

int ServerConfig::getFdListen() const
{
    return this->_fd_listen;
}

const std::vector<Location>::iterator ServerConfig::getLocationKeys(std::string key)
{
	std::vector<Location>::iterator ito;
	for (ito = this->_locations.begin(); ito != this->_locations.end(); ito++)
	{
		if (ito->getPath() == key)
			return (ito);
	}
	throw ErrorException("getLocationKeys: Error: location to key not found");
}

// used to setup sockets and bind them to the server
void    ServerConfig::startServer(void)
{
    if ((_fd_listen = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "webserv: socket() failed %s\t Closing Webserv", strerror(errno));
        exit(EXIT_FAILURE);
    }
    int value_option = 1;
    setsockopt(_fd_listen, SOL_SOCKET, SO_REUSEADDR, &value_option, sizeof(int));
    memset(&_server_address, 0, sizeof(_server_address));
    _server_address.sin_family = AF_INET;
    _server_address.sin_addr.s_addr = _host;
    _server_address.sin_port = htons(_port);
    if (bind(_fd_listen, (struct sockaddr *) &_server_address, sizeof(_server_address)) == -1)
    {
        ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "webserv: bind failed %s\t Closing Webserv", strerror(errno));
        exit(EXIT_FAILURE);
    }
}