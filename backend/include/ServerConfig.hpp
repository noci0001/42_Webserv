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
};

ServerConfig::ServerConfig(/* args */)
{
}

ServerConfig::~ServerConfig()
{
}


#endif