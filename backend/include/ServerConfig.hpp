#ifndef SERVERCONFIG_HPP
# define SERVERCONFIG_HPP

#include "Webserv.hpp"
#include <netinet/in.h>
#include <vector>

//static std::string  serverParameter[] = {"server_name", "listen", "root", "index", "allow_methods", "client_body_buffer_size"};

class Location; //needed for root and directories <-- parsing

class ServerConfig : public Configfile
{
private:
    uint16_t                        _port;
    in_addr_t                       _host;
    std::string                     _server_name;
    std::string                     _root;
    unsigned long                   _max_body_size_client;
    std::string                     _index;
    bool                            _autoindex;
    std::map<short, std::string>    _error_pages;
    std::vector<Location>           _locations;
    struct sockaddr_in                  _server_address;
    int                                 _fd_listen;

public:
    ServerConfig();
    ~ServerConfig();
    ServerConfig( const ServerConfig &other );
    ServerConfig &operator=( const ServerConfig &rhs);

    void    initErrorPages( void );
    void    setServerName(std::string server_name);
    void    setHost(std::string parameter);
    void    setRoot(std::string root);
    void    setPorts(std::string parameter);
    void    setMaxBodySizeClient(std::string parameter);
    void    setIndex(std::string index);
    void    setAutoIndex(std::string autoindex);
    void    setErrorPages(std::vector<std::string> &parameter);
    
    bool    validHost(std::string host) const;

    const std::string &getServerName();
    const std::string &getRoot();
    const bool &getAutoIndex();
    const in_addr_t &getHost();
    const uint16_t &getPort();
    const size_t &getMaxBodySizeClient();
    const std::string &getIndex();
    const std::map<short, std::string> &getErrorPages();
    const std::vector<Location> &getLocations();
    const int &getFdListen();

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