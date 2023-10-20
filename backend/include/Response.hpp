#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <algorithm>
#include <iostream>
#include <string>

#define TRUE 1
#define FALSE 0

static MimeType   mime_types;
static StatusCode status_codes;

class Response {

private:
	// store details of the associated HTTP request
	Request			request; 
	std::string		_contentType;
	//load error page and update body
	void			callErrorPage( std::string &body, std::string error_page );

public:
	Response();
	Response( Request request, Config server_conf );
	~Response();

	std::string statusCode;
	std::string body;
	Config		server_conf; //check naming Tom
	std::string location;	//redirection URL
	Route		using_route; //routing details(endpoint)
};

#endif