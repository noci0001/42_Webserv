#include "../include/Webserv.hpp"
#include "../include/CgiHandler.hpp"

// Default constructor
CgiHandler::CgiHandler()
{
    this->_cgi_pid = -1;
    this->_exit_return = 0;
    this->_cgi_path = "";
    this->_envi_vars_array = NULL;
    this->_argv = NULL;
}

// Copy constructor
CgiHandler::CgiHandler(std::string path)
{
    this->_cgi_pid = -1;
    this->_exit_return = 0;
    this->_cgi_path = path;
    this->_envi_vars_array = NULL;
    this->_argv = NULL;
}

// Destructor: Cleans up dynamically allocated memory for environment variables array
CgiHandler::~CgiHandler()
{
    if (this->_envi_vars_array)
    {
        for (int i = 0; this->_envi_vars_array[i]; i++)
            free(this->_envi_vars_array[i]);
        free(this->_envi_vars_array);
    }
    if (this->_argv)
    {
        for (int i = 0; this->_argv[i]; i++)
            free(this->_argv[i]);
        free(this->_argv);
    }
    this->_envi.clear();
}

CgiHandler::CgiHandler(const CgiHandler &other)
{
    this->_envi = other._envi;
    this->_envi_vars_array = other._envi_vars_array;
    this->_argv = other._argv;
    this->_cgi_path = other._cgi_path;
    this->_cgi_pid = other._cgi_pid;
    this->_exit_return = other._exit_return;
}

CgiHandler &CgiHandler::operator=(const CgiHandler &rhs)
{
    if (this != &rhs)
    {
        this->_envi = rhs._envi;
        this->_envi_vars_array = rhs._envi_vars_array;
        this->_argv = rhs._argv;
        this->_cgi_path = rhs._cgi_path;
        this->_cgi_pid = rhs._cgi_pid;
        this->_exit_return = rhs._exit_return;
    }
    return *this;
}

// Setter Functions
void    CgiHandler::setCgiPath(const std::string &cgi_path)
{
    this->_cgi_path = cgi_path;
}

void    CgiHandler::setCgiPid(pid_t cgi_pid)
{
    this->_cgi_pid = cgi_pid;
}

// Getter Functions
const std::map<std::string, std::string> &CgiHandler::getEnvi() const
{
    return (this->_envi);
}

const std::string &CgiHandler::getCgiPath() const
{
    return (this->_cgi_path);
}

const pid_t &CgiHandler::getCgiPid() const
{
    return (this->_cgi_pid);
}

// Executes the CGI script and returns its output
void CgiHandler::run(short &error_code)
{
	if (this->_argv[0] == NULL || this->_argv[1] == NULL)
	{
		error_code = 500;
		return ;
	}
	if (pipe(pipe_in) < 0)
	{
		ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "CgiHandler: Error: pipe_in failed");
		error_code = 500;
		return ;
	}
	if (pipe(pipe_out) < 0)
	{
		ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "CgiHandler: Error: pipe_out failed");
		close(pipe_in[0]);
		close(pipe_in[1]);
		error_code = 500;
		return ;
	}
	this->_cgi_pid = fork();
	if (this->_cgi_pid == 0)
	{
		dup2(pipe_in[0], STDIN_FILENO);
		dup2(pipe_out[1], STDOUT_FILENO);
		close(pipe_in[0]);
		close(pipe_in[1]);
		close(pipe_out[0]);
		close(pipe_out[1]);
		this->_exit_return = execve(this->_argv[0], this->_argv, this->_envi_vars_array);
		exit(this->_exit_return);
	}
	else if (this->_cgi_pid > 0) {}
	else
	{
		std::cout << "CgiHandler: Error: fork failed" << std::endl;
		error_code = 500;
	}
}

// Creates a map of environment variables based on the provided CGI request
void CgiHandler::createEnviVars(HttpRequest &httprequest, const std::vector<Location>::iterator ito_loca)
{
    std::string run_cgi = ("cgi-bin/" + std::string(ito_loca->getCgiPath()[0]));
    char    *cwd = getcwd(NULL, 0);
    if (_cgi_path[0] != '/')
    {
        std::string temp(cwd);
        temp.append("/");
        if (_cgi_path.length() > 0)
            _cgi_path.insert(0, temp);
    }
    if (httprequest.getMethod() == POST)
    {
        std::stringstream out;
        out << httprequest.getBody().length();
        this->_envi["CONTENT_LENGTH"] = out.str();
        this->_envi["CONTENT_TYPE"] = httprequest.getHeader("content-type");
    }

    // These environment variables are standard for executing CGI scripts
    // and provide the script with information about the request
    // and server environment
    // More information can be found in the CGI specification
    //tmp_env_vars["AUTH_TYPE"]         = _cgi_request.authorization;
    this->_envi["REDIRECT_STATUS"]   = "200";
    this->_envi["GATEWAY_INTERFACE"] = std::string ("CGI/1.1");
    this->_envi["SCRIPT_NAME"]       = run_cgi;
    this->_envi["SCRIPT_FILENAME"]   = this->_cgi_path;
    this->_envi["REQUEST_METHOD"]    = httprequest.getMethodStr();
    //tmp_env_vars["CONTENT_LENGTH"]    = _cgi_request.body; // This might need conversion to char* or length of body
    //tmp_env_vars["CONTENT_TYPE"]      = _cgi_request.content_type;
    this->_envi["PATH_INFO"]         = this->_cgi_path;
    this->_envi["PATH_TRANSLATED"]   = this->_cgi_path;
    //tmp_env_vars["QUERY_STRING"]      = _cgi_request.query;
    //tmp_env_vars["REMOTEaddr"]        = _cgi_request.port;
    //tmp_env_vars["REMOTE_IDENT"]      = _cgi_request.authorization;
    //tmp_env_vars["REMOTE_USER"]       = _cgi_request.authorization;
    this->_envi["REQUEST_URI"]       = this->_cgi_path;
    this->_envi["SERVER_NAME"]       = httprequest.getHeader("host");
    this->_envi["SERVER_PORT"]       = "8002";
    this->_envi["SERVER_PROTOCOL"]   = "HTTP/1.1";
    this->_envi["SERVER_SOFTWARE"]   = "42 Webflix";

    std::map<std::string, std::string> request_headers = httprequest.getHeaders();
    for (std::map<std::string, std::string>::iterator ito = request_headers.begin();
        ito != request_headers.end(); ++ito)
    {
        std::string name = ito->first;
        std::transform(name.begin(), name.end(), name.begin(), ::toupper);
        std::string key = "HTTP_" + name;
        _envi[key] = ito->second;
    }
    this->_envi_vars_array = (char **)calloc(sizeof(char *), this->_envi.size() + 1);
    std::map<std::string, std::string>::const_iterator cito = this->_envi.begin();
    for (int i = 0; cito != this->_envi.end(); cito++, i++)
    {
        std::string temp = cito->first + "=" + cito->second;
        this->_envi_vars_array[i] = strdup(temp.c_str());
    }
    this->_argv = (char **)malloc(sizeof(char *) * 3);
    this->_argv[0] = strdup(run_cgi.c_str());
    this->_argv[1] = strdup(this->_cgi_path.c_str());
    this->_argv[2] = NULL;
}

// Initializes the environment variables array for the CGI script
void	CgiHandler::createEnvi(HttpRequest &httprequest, const std::vector<Location>::iterator ito_loca)
{
	int		position;
	std::string cgi_extension;
	std::string cgi_extension_path;

	cgi_extension = this->_cgi_path.substr(this->_cgi_path.find_last_of("."));
	std::map<std::string, std::string>::iterator ito_path = ito_loca->_extension_path.find(cgi_extension);
	if (ito_path == ito_loca->_extension_path.end())
		return ;
	cgi_extension_path = ito_loca->_extension_path[cgi_extension];

	this->_envi["AUTH_TYPE"]         = "Basic";
	this->_envi["CONTENT_LENGTH"]    = httprequest.getHeader("content-length");
	this->_envi["CONTENT_TYPE"]      = httprequest.getHeader("content-type");
	this->_envi["GATEWAY_INTERFACE"] = "CGI/1.1";
	position = findStart(this->_cgi_path, "cgi-bin/");
	this->_envi["SCRIPT_NAME"]       = this->_cgi_path;
	this->_envi["SCRIPT_FILENAME"]   = ((position < 0 || (size_t)(position + 8) >
	        this->_cgi_path.size()) ? "" : this->_cgi_path.substr(position + 8, this->_cgi_path.size()));
	this->_envi["PATH_INFO"]         = getPathInfo(httprequest.getPath(), ito_loca->getCgiExtension());
	this->_envi["PATH_TRANSLATED"]   = ito_loca->getRoot() + (this->_envi["PATH_INFO"] == "" ? "/" : this->_envi["PATH_INFO"]);
	this->_envi["QUERY_STRING"]      = decodeHttpRequest(httprequest.getQuery());
	this->_envi["REMOTE_ADDR"]        = httprequest.getHeader("host");
	position = findStart(httprequest.getHeader("host"), ":");
	this->_envi["SERVER_NAME"]       = (position > 0 ? httprequest.getHeader("host").substr(0, position) : "");
	this->_envi["SERVER_PORT"]       = (position > 0 ? httprequest.getHeader("host").substr(position + 1, httprequest.getHeader("host").size()) : "");
	this->_envi["REQUEST_METHOD"]    = httprequest.getMethodStr();
	this->_envi["HTTP_COOKIE"]	 	 = httprequest.getHeader("cookie");
	this->_envi["DOCUMENT_ROOT"]     = ito_loca->getRoot();
	this->_envi["REQUEST_URI"]		 = httprequest.getPath() + httprequest.getQuery();
	this->_envi["SERVER_PROTOCOL"]   = "HTTP/1.1";
	this->_envi["REDIRECT_STATUS"]   = "200";
	this->_envi["SERVER_SOFTWARE"]   = "42 Webflix";
	//tmp_env_vars["REMOTE_IDENT"]      = _cgi_request.authorization;
	//tmp_env_vars["REMOTE_USER"]       = _cgi_request.authorization;

	this->_envi_vars_array = (char **) calloc(sizeof (char*), this->_envi.size() + 1);
	std::map<std::string, std::string>::const_iterator cito = this->_envi.begin();
	for (int i = 0; cito != this->_envi.end(); cito++, i++)
	{
		std::string temp = cito->first + "=" + cito->second;
		this->_envi_vars_array[i] = strdup(temp.c_str());
	}
	this->_argv = (char **) malloc(sizeof (char*) * 3);
	this->_argv[0] = strdup(cgi_extension_path.c_str());
	this->_argv[1] = strdup(this->_cgi_path.c_str());
	this->_argv[2] = NULL;
}

int 	CgiHandler::findStart(const std::string path, const std::string delimiter)
{
	if (path.empty())
		return -1;
	size_t	position = path.find(delimiter);
	if (position != std::string::npos)
		return (position);
	else
		return -1;
}

std::string CgiHandler::getPathInfo(std::string &path, std::vector<std::string> cgi_extensions)
{
	std::string temp;
	size_t start, end;

	for (std::vector<std::string>::iterator ito_extension = cgi_extensions.begin();
		ito_extension != cgi_extensions.end(); ++ito_extension)
	{
		start = path.find(*ito_extension);
		if (start != std::string::npos)
			break ;
	}
	if (start == std::string::npos)
		return "";
	if (start + 3 >= path.size())
		return "";
	temp = path.substr(start + 3, path.size());
	if (temp[0] || temp[0] != '/')
		return "";
	end = temp.find("?");
	return (end == std::string::npos ? temp : temp.substr(0, end));
}

std::string CgiHandler::decodeHttpRequest(std::string &path)
{
	size_t token = path.find("%");
	while (token != std::string::npos)
	{
		if (path.length() < token + 2)
			break ;
		char decimal = fromHexToDecimal(path.substr(token + 1, 2));
		path.replace(token, 3, toString(decimal));
		token = path.find("%");
	}
	return path;
}

void	CgiHandler::clearCgiHandler()
{
	this->_cgi_pid = -1;
	this->_exit_return = 0;
	this->_cgi_path = "";
	this->_envi_vars_array = NULL;
	this->_argv = NULL;
	this->_envi.clear();
}
