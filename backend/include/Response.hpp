#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "Webserv.hpp"
#include "MimeType.hpp"
#include "HttpRequest.hpp"
#include "ServerConfig.hpp"
#include "CgiHandler.hpp"

struct Route
{
    std::vector<std::string> allow_methods;
    std::string              root;
    std::string              index;
    std::vector<std::string> cgi;
    std::string              location_dir;
    int                      client_max_body_size;
    std::vector<std::string> http_redirection;
    std::string              autoindex; // directory listing
};

struct Config
{
    int                      listen_port;
    std::string              server_name;
    int                      client_max_body_size;
    std::vector<std::string> index;
    std::vector<std::string> allowed_method;
    std::vector<std::string> error_page;
    std::string              autoindex;
    std::vector<Route>       routes;
    std::string              root_dir;

    Config();

    Config( std::vector<std::string>::iterator &it,
            std::vector<std::string>::iterator &end );

    void setRouteAttributes( Route &route, std::string &line );

    void setConfig( std::string &line );
};

struct Request
{
    std::string method; // GET
    std::string host; // hostname
    std::string port; // port
    std::string uri; // /index.html
    std::string version; // HTTP/1.1
    std::string body; // request body
    std::string content_type; // text/html
    std::string content_length;
    std::string user_agent; // Mozilla/5.0 (X11; Linux x86_64; rv:78.0)
    std::string authorization;
    std::string accept;
    std::string query;
    std::string cgi_path;

    Request();

    Request( const char *buf );

    void setMethod( std::string &line );
    void setHost( std::string &line );
    void setUserAgent( std::string &line );
    void setAccept( std::string &line );
    void setConcentLength( std::string &line );
    void setContentType( std::string &line );
    void setBody( const char *buf );
    void display();
};

class Response 
{

	public:
		static	MimeType	_mime_type;
		Response();
		Response(HttpRequest&);
		~Response();


		std::string	getResponse();
		size_t		getResponseLength() const;
		int			getStatusCode() const;

		void		setHttpRequest(HttpRequest &);
		void		setServerConfig(ServerConfig &);

		void		buildResponse();
		void		clearResponse();
		void		handleCgi(HttpRequest &);
		void		cutResponse(size_t);
		int			getCgiState();
		void		setCgiState(int);
		void		setErrorResponse(short error_code);

		CgiHandler	_cgiHandler;

		std::string	removeBoundary(std::string &body, std::string &boundary);
		std::string	_responseContent;

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
		

		int						buildBody();
		size_t 					file_size();
		void					setStatusCode();
		void 					setHeaders();
		void					setDefaultErrorPages();
		int						readFile();
		void					contentType();
		void					contentLength();
		void					currentConnection();
		void					server();
		void					location();
		void					date();
		int						targetHandle();
		void					buildBodyError();
		bool					requestError();
		int						CgiHandling(std::string &);
		int						CgiHandlingTmp(std::string &);
};

#endif