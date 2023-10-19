#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include "Webserv.hpp"

static std::string  serverParameter[] = {"server_name", "listen", "root", "index", "allow_methods", "client_body_buffer_size"};

class Location; //needed for root and directories <-- parsing

class ServerConfig
{
private:
    uint16_t                        _port;
    in_addr_t                       _host;
    std::string                     _server_name;
    std::string                     _root;
    unsigned long                   _max_body_size_client;
    std::string                     _index;
    bool                            _autoindex;
    std::map <short, std::string>   _error_pages;
    std::vector <Location>          _locations;

struct sockaddr_in                  _server_address;
int                                 _fd_listen;

public:
    ServerConfig();
    ~ServerConfig();
    ServerConfig( const ServerConfig &other );
    ServerConfig &operator=( const ServerConfig &rhs);

    void    initErrorPages( void );
    void    ServerConfig::setServerName(std::string server_name);
    void    ServerConfig::setHost(std::string parameter);
    void    ServerConfig::setRoot(std::string root);
    void    ServerConfig::setPorts(std::string parameter);
    void    ServerConfig::setMaxBodySizeClient(std::string parameter);
    void    ServerConfig::setIndex(std::string index);
    void    ServerConfig::setAutoIndex(std::string autoindex);
    void    ServerConfig::setErrorPages(std::vector<std::string> &parameter);

    public:
        class ErrorException : public std::exception
        {
            private:
                std::string _message;
            public:
                ErrorException(std::string message) throw()
                {
                    _message = "ServerConfig Error: " + message;
                }
                virtual const char* what() const throw()
                {
                    return (_message.c_str());
                }
                virtual ~ErrorException() throw() {}
        };
};

#endif