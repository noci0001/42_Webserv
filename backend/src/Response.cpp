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
	else if (path == "cgi-bin/")
		cgi_path.append(_server_config.getLocations(location_key)->getIndex());
	pos = cgi_path.find(".");
	if (pos == std::string::npos)
	{
		_status_code = 501;
		return 1;
	}
	cgi_extension = cgi_path.substr(pos);
	if (cgi_extension != ".py" && cgi_extension != ".sh")
	{
		_status_code = 501;
		return 1;
	}
	if (Configfile::getTypePath(cgi_path) != 1)
	{
		_status_code = 404;
		return 1;
	}
	if (Configfile::checkFile(cgi_path, 1) == -1 || Configfile::checkFile(cgi_path, 3) == -1)
	{
		_status_code = 403;
		return 1;
	}
	if (isAllowedMethod(httpRequest.getMethod(), *_server_config.getLocations(location_key), _status_code))
		return 1;
	_cgiHandler.clear();
	_cgiHandler.setCgiPath(cgi_path);
	_cgi_state = 1;
	if (pipe(_cgi_fd) < 0)
	{
		_status_code = 500;
		return 1;
	}
	_cgiHandler.initEnvCgi(httpRequest, _server_config.getLocations(location_key));
	_cgiHandler.execute(this->_status_code);
	return 0;
}

static void	getLocationMatch(std::string &path, std::vector<Location> location, std::string location_key)
{
	size_t	max_match = 0;
	for (std::vector<Location>::const_iterator cito = location.begin(); cito != location.end(); ++cito)
	{
		if (path.find(cito->getPath()) == 0)
		{
			if (cito->getPath() == "/" || path.length() == cito->getPath().length() || path[it->getPath().length()] == '/')
			{
				max_match = cito->getPath().length();
				location_key = cito->getPath();
			}
		}
	}
}

int Response::targetHandle()
{
	std::string location_key;
	getLocationMatch(httpRequest.getPath(), _server_config.getLocations(), location_key);
	if (location_key.length() > 0)
	{
		Location target_location = *_server_config.getLocations(location_key);
		if (isAllowedMethod(httpRequest.getMethod(), target_location, _status_code))
		{
			std::cout << "Method is not allowed" << std::endl;
			return 1;
		}
		if (httpRequest.getBody().length() > target_location.getMaxBodySizeClient())
		{
			_status_code = 413;
			return 1;
		}
		if (statusReturn(target_location, _status_code, _location))
			return 1;
		if (target_location.getPath().find("cgi-bin") != std::string::npos)
			return CgiHandling(location_key);
		if (!target_location.getAlias().empty())
			replaceAlias(target_location, httpRequest, _target_file);
		else
			replaceRoot(target_location, httpRequest, _target_file);
		if (!target_location.getCgiExtension().empty())
		{
			if (_target_file.rfind(target_location.getCgiExtension()[0]) != std::string::npos)
			{
				return CgiHandlingTmp(location_key);
			}
		}
		if (isDirectory(_target_file))
		{
			if (_target_file[_target_file.length() - 1] != '/')
			{
				_status_code = 301;
				_location = httpRequest.getPath() + "/";
				return 1;
			}
			if (!target_location.getIndex().empty())
				_target_file.append(target_location.getIndex());
			else
				_target_file.append(_server_config.getIndex());
			if (!fileExists(_target_file))
			{
				if (target_location.getAutoIndex())
				{
					_target_file.erase(_target_file.find_last_of('/') + 1);
					_auto_index = true;
					return 0;
				}
				else
				{
					_status_code = 403;
					return 1;
				}
			}
		}
		else
		{
			_target_file = pairPaths(_server_config.getRoot(), httpRequest.getPath(), "");
			if (isDirectory(_target_file))
			{
				if (_target_file[_target_file.length() - 1] != '/')
				{
					_status_code = 301;
					_location = httpRequest.getPath() + "/";
					return 1;
				}
				_target_file += _server_config.getIndex();
				if (!fileExists(_target_file))
				{
					_status_code = 301;
					return 1;
				}
				if (isDirectory(_target_file))
				{
					_status_code = 301;
					_location = pairPaths(httpRequest.getPath(), _server_config.getIndex(), "/");
					if (_location[_location.length() - 1] != '/')
						_location.insert(_location.end(), '/');
					return 1;
				}
			}
		}
	}
	return 0;
}

bool Response::requestError()
{
	if (httpRequest.errorCode())
	{
		_status_code = httpRequest.errorCode();
		return 1;
	}
	return 0;
}

void	Response::setDefaultErrorPages()
{
	_body_response = getErrorPage(_status_code);
}

void	Response::buildBodyError()
{
	if (!_server_config.getErrorPages().count(_status_code) || _server_config.getErrorPages().at(_status_code).empty()
		|| httpRequest.getMethod() == DELETE || httpRequest.getMethod() == POST)
	{
		setDefaultErrorPages();
	}
	else
	{
		if (_status_code >= 400 && _status_code < 500)
		{
			_location = _server_config.getErrorPages().at(_status_code);
			if (_location[0] != '/')
				_location.insert(_location.begin(), '/');
			_status_code = 302;
		}
		_target_file = _server_config.getRoot() + _server_config.getErrorPages().at(_status_code);
		short old_status_code = _status_code;
		if (readFile())
		{
			_status_code = old_status_code;
			_body_response = getErrorPage(_status_code);
		}
	}
}

void	Response::buildResponse()
{
	if (requestError() || buildBody())
		buildBodyError();
	if (_cgi_state)
		return ;
	else if (_auto_index)
	{
		std::cout << "Auto index " << std::endl;
		if (buildHtmlIndex(_target_file, _body, _body_length))
		{
			_status_code = 500;
			buildBodyError();
		}
		else
			_status_code = 200;
		_body_response.insert(_body_request.begin(), _body.begin(), _body.end());
	}
	setStatusCode();
	setHeaders();
	if (httpRequest.getMethod() != HEAD && (httpRequest.getMethod() == GET || _status_code != 200))
		_responseContent.append(_body_response);
}

void	Response::setErrorResponse(short error_code)
{
	_responseContent = "";
	_status_code = error_code;
	_body_response = "";
	buildBodyError();
	setStatusCode();
	setHeaders();
	_responseContent.append(_body_response);
}

std::string	Response::getResponse()
{
	return _responseContent;
}

size_t	Response::getResponseLength() const
{
	return _responseContent.length();
}

void	Response::setStatusCode()
{
	_responseContent.append("HTTP/1.1 " + toString(_status_code) + " ");
	_responseContent.append(statusCodeString(_status_code));
	_responseContent.append("\r\n");
}

int	Response::buildBody()
{
	if (httpRequest.getBody().length() > _server_config.getMaxBodySizeClient())
	{
		_status_code = 413;
		return 1;
	}
	if (targetHandle())
		return 1;
	if (_cgi_state || _auto_index)
		return 0;
	if (_status_code)
		return 0;
	if (httpRequest.getMethod() == GET || httpRequest.getMethod() == HEAD)
	{
		if (readFile())
			return 1;
	}
	else if (httpRequest.getMethod() == POST || httpRequest.getMethod() == PUT)
	{
		if (fileExists(_target_file) && httpRequest.getMethod() == POST)
		{
			_status_code = 204;
			return 0;
		}
		std::ofstream file(_target_file.c_str(), std::ios::binary);
		if (file.fail())
		{
			_status_code = 404;
			return 1;
		}
		if (httpRequest.getMultiformFlag())
		{
			std::string body = httpRequest.getBody();
			body = removeBoundary(body, httpRequest.getBoundary());
			file.write(body.c_str(), body.length());
		}
		else
			file.write(httpRequest.getBody().c_str(), httpRequest.getBody().length());
	}
	else if (httpRequest.getMethod() == DELETE)
	{
		if (!fileExists(_target_file))
		{
			_status_code = 404;
			return 1;
		}
		if (remove(_target_file.c_str()) != 0)
		{
			_status_code = 500;
			return 1;
		}
	}
	_status_code = 200;
	return 0;
}