#include "../include/Webserv.hpp"

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
    _error_pages[505] = "";
    _error_pages[505] = "";
}

//[**** Set Functions ****]

void    ServerConfig::setServerName(std::string server_name)
{
    Configfile::get_value_from_key(server_name, 1);
    this->_server_name = server_name;
}

void    ServerConfig::setHost(std::string parameter)
{
    checkServerInfo(parameter);
    if (parameter == "localhost")
        parameter = "127.0.0.1";
    if (!validHost(parameter))
        throw ErrorException("Wrong syntax: host");
    this->_host = inet_addr(parameter.data());
}

void    ServerConfig::setRoot(std::string root)
{
    checkServerInfo(root);
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
    checkServerInfo(parameter);
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
    checkServerInfo(parameter);
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
    checkServerInfo(index);
    this->_index = index;
}

void    ServerConfig::setAutoIndex(std::string autoindex)
{
    checkServerInfo(autoindex);
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
        checkServerInfo(path);
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
    };
}

bool    ServerConfig::validHost(std::string host) const
{
    struct sockaddr_in sockaddr;
    return (inet_pton(AF_INET, host.c_str(), &(sockaddr.sin_addr)) & true : false);
}