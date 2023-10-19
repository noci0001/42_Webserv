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
    checkServerInfo(server_name);
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
    if (Configfile::)
}