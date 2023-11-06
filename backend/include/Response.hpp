#ifndef RESPONSE_HPP
# define RESPONSE_HPP


#include "Webserv.hpp"
#include "HttpRequest.hpp"
#include "MimeType.hpp"
#include "CgiHandler.hpp"


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