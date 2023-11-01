#include "../include/Response.hpp"

Mime Response::mime;

Response::Response()
{
	_target_file = "";
	_body.clear();
	_body_length = 0;
	_body_response = "";
	_responseContent = "";
	_location = "";
	_status_code = 0;
	_cgi_state = 0;
	_cgi_bytes_read = 0;
	_auto_index = 0;
}

Response::~Response()	{}

Response::Response(HttpRequest &request) : httpRequest(request)
{
	_target_file = "";
	_body.clear();
	_body_length = 0;
	_body_response = "";
	_responseContent = "";
	_location = "";
	_status_code = 0;
	_cgi_state = 0;
	_cgi_bytes_read = 0;
	_auto_index = 0;
}

void	Response::contentType()
{
	_responseContent.append("Content-Type: ");
	if (_target_file.rfind(".", std::string::npos) != std::string::npos && _status_code == 200)
		_responseContent.append(mime.getMimeType(_target_file.substr(_target_file.rfind(".", std::string::npos))));
	else
		_responseContent.append(mime.getMimeType("default"));
	_responseContent.append("\r\n");
}

void	Response::contentLength()
{
	std::stringstream ss;
	ss << _body_response.length();
	_responseContent.append("Content-Length: ");
	_responseContent.append(ss.str());
	_responseContent.append("\r\n");
}

void	Response::currentConnection()
{
	if (httpRequest.getHeader("Connection") == "keep-alive")
		_responseContent.append("Connection: keep-alive\r\n");
}

void 	Response::server()
{
	_responseContent.append("Server: 42 Webflix\r\n");
}

void	Response::location()
{
	if (_location.length())
		_responseContent.append("Location: " + _location + "\r\n");
}

void	Response::date()
{
	char date[1000];
	time_t current = time(0);
	struct tm tm = *gmtime(&current);
	strftime(date, sizeof(date), "%a, %d %b %Y %H:%M:%S %Z", &tm);
	_responseContent.append("Date: ");
	_responseContent.append(date);
	_responseContent.append("\r\n");
}

void	Response::setHeaders()
{
	contentType();
	contentLength();
	currentConnection();
	server();
	location();
	date();
	_responseContent.append("\r\n");
}

static bool		fileExists( const std::string &filename )
{
	std::ifstream file(filename.c_str());
	return file.good();
}

static bool		isDirectory( std::string path )
{
	struct stat stat_file;
	if (stat(path.c_str(), &stat_file) != 0)
		return false;
	return (S_ISDIR(stat_file.st_mode));
}

static bool		isAllowedMethod(HttpMethod &method, Location &location, short &status_code)
{
	std::vector<short> methods = location.getMethods();
	if ((method == GET && !methods[0]) || (method == POST && !methods[1])
		(method == DELETE && !methods[2]) || (method == PUT && !methods[3])
		(method == HEAD && !methods[4]))
	{
		status_code = 405;
		return false;
	}
	return true;
}

static bool		statusReturn(Location &locat, short &status_code, std::string &location)
{
	if (!locat.getReturn().empty())
	{
		status_code = 301;
		location = locat.getReturn();
		if (location[0] != '/')
			location.insert(location.begin(), '/');
		return false;
	}
	return true;
}

static std::string pairPaths(std::string path1, std::string path2, std::string path3)
{
	std::string	result;
	int			length1;
	int			length2;

	length1 = path1.length();
	length2 = path2.length();
	if (path1[length1 - 1] == '/' && (!path2.empty() && path2[0] == '/'))
		path2.erase(0, 1);
	if (path1[length1 - 1] != '/' && (!path2.empty() && path2[0] != '/'))
		path1.insert(path1.end(), '/');
	if (path2[length2 - 1] == '/' && (!path3.empty() && path3[0] == '/'))
		path3.erase(0, 1);
	if (path2[length2 - 1] != '/' && (!path3.empty() && path3[0] != '/'))
		path2.insert(path1.end(), '/');
	result = path1 + path2 + path3;
	return result;
}

static void		replaceAlias(Location &location, HttpRequest &httprequest, std::string &target_file)
{
	target_file = pairPaths(location.getAlias(),
	httprequest.getPath().substr(location.getPath().length()), "");
}

static void		replaceRoot(Location &location, HttpRequest &httprequest, std::string &target_file)
{
	target_file = pairPaths(location.getRoot(), httprequest.getPath(), "");
}

int 	Response::CgiHandlingTmp(std::string &location_key)
{
	std::string	path;
	path = _target_file;
	_cgiHandler.clear();
	_cgiHandler.setCgiPath(path);
	_cgi_state = 1;
	if (pipe(_cgi_fd) < 0)
	{
		_status_code = 500;
		return 1;
	}
	_cgiHandler.initEnvCgi(httprequest, _server_config.getLocations(location_key));
	_cgiHandler.execute(this->_status_code);
	return 0;
}

int 	Response::CgiHandling(std::string &location_key)
{
	std::string cgi_path;
	std::string cgi_extension;
	size_t 		pos;

	cgi_path = this->httpRequest.getPath();
	if (cgi_path[0] && cgi_path[0] == '/')
		cgi_path.erase(0, 1);
	if (cgi_path == "cgi-bin")
		cgi_path += "/" + _server_config.getLocations(location_key)->getIndex();
}