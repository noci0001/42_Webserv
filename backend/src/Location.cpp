
#include "../include/Webserv.hpp"
#include "../include/Location.hpp"

Location::Location()
{
    this->_path = "";
    this->_root = "";
    this->_autoindex = false;
    this->_index = "";
    this->_methods.reserve(5);
    this->_methods.push_back(1);
    this->_methods.push_back(0);
    this->_methods.push_back(0);
    this->_methods.push_back(0);
    this->_methods.push_back(0);
    this->_return = "";
    this->_alias = "";
    this->_max_body_size_client = MAX_CONTENT_LENGTH;
}

Location::Location(const Location &other)
{
    this->_path = other._path;
    this->_root = other._root;
    this->_autoindex = other._autoindex;
    this->_index = other._index;
    this->_methods = other._methods;
    this->_return = other._return;
    this->_alias = other._alias;
    this->_max_body_size_client = other._max_body_size_client;
    this->_extension_path = other._extension_path;
    this->_cgi_path = other._cgi_path;
    this->_cgi_extension = other._cgi_extension;
}

Location &Location::operator=(const Location &rhs)
{
    if (this != &rhs)
    {
        this->_path = rhs._path;
        this->_root = rhs._root;
        this->_autoindex = rhs._autoindex;
        this->_index = rhs._index;
        this->_methods = rhs._methods;
        this->_return = rhs._return;
        this->_alias = rhs._alias;
        this->_max_body_size_client = rhs._max_body_size_client;
        this->_extension_path = rhs._extension_path;
        this->_cgi_path = rhs._cgi_path;
        this->_cgi_extension = rhs._cgi_extension;
    }
    return *this;
}

Location::~Location()   {}

//[**** Setter Functions ****]
void    Location::setPath(std::string parameter)
{
    this->_path = parameter;
}

void    Location::setRoot(std::string parameter)
{
    if (Configfile::getTypePath(parameter) != 2)
        throw ServerConfig::ErrorException("location of root directory");
    this->_root = parameter;
}

void    Location::setMethods(std::vector<std::string> methods)
{
    this->_methods[0] = 0;
    this->_methods[1] = 0;
    this->_methods[2] = 0;
    this->_methods[3] = 0;
    this->_methods[4] = 0;

    for (size_t i = 0; i < methods.size(); i++)
    {
        if (methods[i] == "GET")
            this->_methods[0] = 1;
        else if (methods[i] == "POST")
            this->_methods[1] = 1;
        else if (methods[i] == "DELETE")
            this->_methods[2] = 1;
        else if (methods[i] == "PUT")
            this->_methods[3] = 1;
        else if (methods[i] == "HEAD")
            this->_methods[4] = 1;
        else
            throw ServerConfig::ErrorException("method not supported " + methods[i]);
    }
}

void    Location::setAutoIndex(std::string parameter)
{
    if (parameter == "on" || parameter == "off")
        this->_autoindex = (parameter == "on");
    else
        throw ServerConfig::ErrorException("incorrect autoindex");
}

void    Location::setIndex(std::string parameter)
{
    this->_index = parameter;
}

void    Location::setReturn(std::string parameter)
{
    this->_return = parameter;
}

void    Location::setAlias(std::string parameter)
{
    this->_alias = parameter;
}

void    Location::setCgiPath(std::vector<std::string> path)
{
    this->_cgi_path = path;
}

void    Location::setCgiExtension(std::vector<std::string> extension)
{
    this->_cgi_extension = extension;
}

void    Location::setMaxBodySizeClient(std::string parameter)
{
	unsigned long   body_size = 0;

    for (size_t i = 0; i < parameter.length(); i++)
    {
        if (parameter[i] < '0' || parameter[i] > '9')
            throw ServerConfig::ErrorException("Wrong Syntax: max_body_size_client");
    }
    if (!ft_stoi(parameter))
        throw ServerConfig::ErrorException("Wrong Syntax: max_body_size_client");
    body_size = ft_stoi(parameter);
    this->_max_body_size_client = body_size;
}

void    Location::setMaxBodySizeClient(unsigned long body_size)
{
    this->_max_body_size_client = body_size;
}

//[**** Getter Functions ****]

const std::string &Location::getPath() const
{
    return this->_path;
}

const std::string &Location::getRoot() const
{
    return this->_root;
}

const std::string &Location::getIndex() const
{
    return this->_index;
}

const std::vector<short> &Location::getMethods() const
{
    return this->_methods;
}

const std::vector<std::string> &Location::getCgiPath() const
{
	return this->_cgi_path;
}

const std::vector<std::string> &Location::getCgiExtension() const
{
	return this->_cgi_extension;
}

const bool  &Location::getAutoIndex() const
{
    return this->_autoindex;
}

const std::string &Location::getReturn() const
{
    return this->_return;
}

const std::string &Location::getAlias() const
{
    return this->_alias;
}

const std::map<std::string, std::string> &Location::getExtensionPath() const
{
    return this->_extension_path;
}

const unsigned long &Location::getMaxBodySizeClient() const
{
    return this->_max_body_size_client;
}

//[* For Printing valid Methods *]
std::string Location::getPrintMethods() const
{
    std::string result;
    if (_methods[4])
        result.insert(0, "HEAD");
    if (_methods[3])
    {
        if (!result.empty())
            result.insert(0, ", ");
        result.insert(0, "PUT");
    }
    if (_methods[2])
    {
        if (!result.empty())
            result.insert(0, ", ");
        result.insert(0, "DELETE");
    }
    if (_methods[1])
    {
        if (!result.empty())
            result.insert(0, ", ");
        result.insert(0, "POST");
    }
    if (_methods[0])
    {
        if (!result.empty())
            result.insert(0, ", ");
        result.insert(0, "GET");
    }
    return result;
}


