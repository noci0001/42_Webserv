#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "CgiHandler.hpp"
#include "Mime.hpp"
#include "StatusCode.hpp"
//#include "utils.hpp"
#include "webserv.hpp"
#include <cerrno>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>

#include <algorithm>
#include <iostream>
#include <string>

#define TRUE 1
#define FALSE 0

static MimeType   mime_types;
static StatusCode status_codes;

namespace ft {

class Response {

private:
	// store details of the associated HTTP request
	Request			request; 
	std::string		_contentType;
	//load error page and update body
	void			callErrorPage( std::string &body, std::string error_page );

public:
	Response();
	Response( Request request, Config serverConf );
	~Response();

	std::string statusCode;
	std::string body;
	Config		serverConfig;
	std::string location;	//redirection URL
	Route		using_route; //routing details(endpoint)

	int         getContentLength();
    void        setStatusCode( std::string code );
    void        setBody( std::string body );
    void        setLocation( std::string location );
    void        setContentType( std::string type );
    std::string getContentType();
    std::string makeResponse();
    int         isValidMethod( std::string              method,
                               std::vector<std::string> allowed_methods );
    void createDirectoryListingIntoHTML( std::string path, std::string &body );
    std::string getPath( std::string uri );
    int         canAutoIndex( std::string path );
    int         checkRedirect();
    void        handleGet();
    std::string generateFileName();
    void        handlePost();
    void        handleDelete();
    int         isLocation( std::string path );
};
}
#endif