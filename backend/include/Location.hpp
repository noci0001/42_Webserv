#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <vector>
#include <map>
#include "Webserv.hpp"
#include "ServerConfig.hpp"

class Location : public Configfile
{
    private:
        std::string         _path;
        std::string         _root;
        bool                _autoindex;
        std::string         _index;
        std::vector<short>  _methods; // GET, POST, DELETE, PUT, HEAD
        std::string         _return;
        std::string         _alias;
		std::vector<std::string>    _cgi_path;
		std::vector<std::string>    _cgi_extension;

        unsigned long       _max_body_size_client;

    public:
        std::map<std::string, std::string>  _extension_path;

        Location();
        Location(const Location &other);
        Location &operator=(const Location &rhs);
        ~Location();

        //[**** Setter Functions ****]
        void    setPath(std::string path);
        void    setRoot(std::string parameter);
        void    setMethods(std::vector<std::string> methods);
        void    setAutoIndex(std::string parameter);
        void    setIndex(std::string parameter);
        void    setReturn(std::string parameter);
        void    setAlias(std::string parameter);
		    void    setCgiPath(std::vector<std::string> path);
		    void    setCgiExtension(std::vector<std::string> extension);
        void    setMaxBodySizeClient(std::string parameter);
        void    setMaxBodySizeClient(unsigned long parameter);

        //[**** Getter Functions ****]
        const std::string &getPath() const;
        const std::string &getRoot() const;
        const std::string &getIndex() const;
        const std::vector<short> &getMethods() const;
        const bool &getAutoIndex() const;
		    const std::vector<std::string> &getCgiPath() const;
		    const std::vector<std::string> &getCgiExtension() const;
        const bool  &getAutoindex() const;
        const std::string &getReturn() const;
        const std::string &getAlias() const;
        const std::map<std::string, std::string> &getExtensionPath() const;
        const unsigned long &getMaxBodySizeClient() const;
        
        //[* For Printing valid Methods *]
        std::string getPrintMethods() const;
};

#endif