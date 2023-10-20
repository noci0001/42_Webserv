#include "../include/Webserv.hpp"

//default constructor
Response::Response(){}
//deconstructor
Response::~Response() {}
//reference to an HttpRequest object and initialization of the request member
Response::Response(HttpRequest &req) : request(req) {}

MimeType::MimeType()
{
	_mimeTypes[".css"] = "text/css";
	_mimeTypes[".csv"] = "text/csv";
	_mimeTypes[".doc"] = "application/msword";
	_mimeTypes[".docx"]
		= "application/"
		  "vnd.openxmlformats-officedocument.wordprocessingml.document";
	_mimeTypes[".epub"]   = "application/epub+zip";
	_mimeTypes[".gz"]	 = "application/gzip";
	_mimeTypes[".gif"]	= "image/gif";
	_mimeTypes[".htm"]	= "text/html";
	_mimeTypes[".html"]   = "text/html";
	_mimeTypes[".ico"]	= "image/vnd.microsoft.icon";
	_mimeTypes[".ics"]	= "text/calendar";
	_mimeTypes[".jpeg"]   = "image/jpeg";
	_mimeTypes[".jpg"]	= "image/jpeg";
	_mimeTypes[".js"]	 = "text/javascript";
	_mimeTypes[".json"]   = "application/json";
	_mimeTypes[".jsonld"] = "application/ld+json";
	_mimeTypes[".mp3"]	= "audio/mpeg";
	_mimeTypes[".mpeg"]   = "video/mpeg";
	_mimeTypes[".png"]	= "image/png";
	_mimeTypes[".pdf"]	= "application/pdf";
	_mimeTypes[".php"]	= "application/x-httpd-php";
	_mimeTypes[".rar"]	= "application/vnd.rar";
	_mimeTypes[".svg"]	= "image/svg+xml";
	_mimeTypes[".txt"]	= "text/plain";
}

MimeType::~MimeType() {}

std::string MimeType::getMimeType( std::string extension )
{
	return _mimeTypes[extension];
}

StatusCode::StatusCode()
{
	_statusCode[200] = "200 OK";
	_statusCode[201] = "201 Created";
	_statusCode[202] = "202 Accepted";
	_statusCode[204] = "204 No Content";
	_statusCode[300] = "300 Multiple Choice";
	_statusCode[301] = "301 Moved Permanently";
	_statusCode[302] = "302 Found";
	_statusCode[400] = "400 Bad Request";
	_statusCode[401] = "401 Unauthorized";
	_statusCode[403] = "403 Forbidden";
	_statusCode[404] = "404 Not Found";
	_statusCode[405] = "405 Method Not Allowed";
	_statusCode[409] = "409 Conflict";
	_statusCode[413] = "413 Request Entity Too Large";
	_statusCode[415] = "415 Unsupported Media Type";
	_statusCode[500] = "500 Internal Server Error";
	_statusCode[502] = "502 Bad Gateway";
	_statusCode[504] = "504 Gateway Timeout";
	_statusCode[505] = "505 HTTP Version Not Supported";
}

StatusCode::~StatusCode() {}

std::string StatusCode::getStatusCode( int code ) 
{ 
	return _statusCode[code];
}

void Response::setStatusCode( std::string code )
{
	this->statusCode = code;
}

void Response::setContentType( std::string type )
{
	this->_contentType = type;
}

void Response::setBody( std::string body ) 
{
	this->body = body;
}

// checks if a provided method is within a list of allowed methods.
//If the method is valid, it returns TRUE. Otherwise, it logs a warning and returns FALSE.
int Response::isValidMethod( std::string			  method,
								 std::vector<std::string> allowed_methods )
{
	for ( std::vector<std::string>::iterator it  = allowed_methods.begin(),
											 end = allowed_methods.end();
		  it != end;
		  ++it ) {
		if ( method == *it ) {
			return TRUE;
		}
	}
	message.warning( "Invalid method invoked by client: " + method );
	return FALSE;
}


Response::Response( Request request, Config serverConfig ) :
	request( request ), server_conf( serverConfig )
{
	//host verification. checks if the server_name from the configuration 
	//matches the host from the request. If not, set the response status 
	//code to 400 (Bad Request), mention the content type, 
	//and set the response body to "Invalid Host \n"
	if ( server_conf.server_name != request.host ) {
		setStatusCode( status_codes.getStatusCode( 400 ) );
		setContentType( "text/plain" );
		setBody( "Invalid Host \n" );
		return;
	}
	//checks if the request body size exceeds the maximum allowed size as 
	//defined in the configuration. If the client_max_body_size is set (not -1)
	//and the request body size is larger than this value, the response status
	//code is set to 413 (Payload Too Large), the content type is mentioned, and
	//an appropriate message is set in the body.
	if ( server_conf.client_max_body_size != -1
		 && static_cast<int>( request.body.size() )
			 > static_cast<int>( server_conf.client_max_body_size ) ) {
		setStatusCode( status_codes.getStatusCode( 413 ) );
		setContentType( "text/plain" );
		setBody( "Request body size exceeds the limit.\n" );
		return;
	}
	// checks if the uri in the request is neither the root ("/")
	//nor an empty string, and it corresponds to a valid location (isLocation).
	//If this is the case, it then checks if the method in the request 
	//(like GET, POST, etc.) is valid for that specific route using isValidMethod(). 
	//If the method is invalid, it sets the response status code to 405 (Method Not Allowed)
	if ( request.uri != "/" && request.uri != ""
		 && isLocation( request.uri ) ) {
		if ( !isValidMethod( request.method, using_route.allow_methods ) ) {
			setStatusCode( status_codes.getStatusCode( 405 ) );
			setBody( "Method not allowed \n" );
			return;
		}
	}
	//checks if the method in the request is valid server-wide using the 
	//isValidMethod() function and the allowed_method from the server configuration.
	//If the method is not allowed, the constructor sets a 405 status code and exits early.
	if ( !isValidMethod( request.method, server_conf.allowed_method ) ) {
		setStatusCode( status_codes.getStatusCode( 405 ) );
		setBody( "Method not allowed \n" );
		return;
	}

	//check the method in the request and call the appropriate handler

	if ( request.method == "POST" ) {
		handlePost();
	}

	if ( request.method == "GET" ) {
		handleGet();
	}

	if ( request.method == "DELETE" ) {
		handleDelete();
	}
}

std::string Response::makeResponse()
{
	std::string response;
	response.append( "HTTP/1.1 " );
	response.append( statusCode );
	response.append( "\r\n" );
	response.append( "Content-Type: " );
	response.append( getContentType() );
	response.append( "\r\n" );
	response.append( "Content-Length: " );
	response.append( NumberToString( getContentLength() ) );
	response.append( "\r\n" );
	response.append( "Connection: keep-alive" );
	response.append( "\r\n\r\n" );

	if ( location.length() > 0 ) {
		response.append( "Location: " );
		response.append( location );
		response.append( "\r\n" );
	}

	response.append( body );

	return response;
}