#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Webserv.hpp"
#include "HttpRequest.hpp"

class Response 
{

	public:
		static	Mime	mime;
		Response();
		Response( HttpRequest & );
		~Response();

		std::string	getResponse();
		size_t		getResponseLength();
		int			getStatusCode();

		void		setHttpRequest( HttpRequest & );
		void		setServerConfig( ServerConfig & );

		void		buildResponse();
		void		clearResponse();
		void		handleCgi( HttpRequest & );
		void		cutResponse( size_t );
		int			getCgiState();
		void		setCgiState( int );
		void		setErrorResponse( short error_code );

		CgiHandler	cgiHandler;

		std::string	removeBoundary( std::string &body, std::string &boundary );
		std::string	responseContent;

		HttpRequest	httpRequest;

	private:
		ServerConfig			_server_config;
		std::string				_target_file;
		std::vector<uint8_t>	_body;
		size_t					_body_length;
		std::string				_body_response;
		std::string				_location;
		short					_status_code;
		char					*_response;
		int						_cgi_state;
		int						_cgi_fd[2];
		size_t					_cgi_bytes_read;
		bool					_auto_index;
		

		
};

#endif