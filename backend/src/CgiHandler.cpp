#include "../include/Webserv.hpp"

// Define a buffer size for CGI responses
#define CGI_BUFSIZE 8192

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
    this->_cgi_path = "";
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

// Executes the CGI script and stores the response in _response_body
void CgiHandler::run(short &error_code)
{
    _response_body = exec_cgi(_cgi_request.cgi_path);
}

// Creates a map of environment variables based on the provided CGI request
void CgiHandler::create_env_vars(HttpRequest &httprequest, const std::vector<Location>::iterator ito_loca)
{
    std::string run_cgi = ("cgi-bin/" + ito_loca->getCgiPath()[0].c_str);
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
    this->_envi["REQUEST_METHOD"]    = httprequest.getMethodStr;
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
    this->_argv[1] = strdup(this->_cgi_path.c_str);
    this->_argv[2] = NULL;
}

// Converts the map of environment variables into an array for execve function
char **CgiHandler::_create_env_vars_array(std::map<std::string, std::string> env_vars)
{
    int size = env_vars.size();
    // Create a new array with size of map + 1 (for null terminator)
    char **result = new char*[size + 1];
    std::map<std::string, std::string>::const_iterator it = env_vars.begin();

    for (int i = 0; it != env_vars.end(); it++, i++)
    {
        std::string env_var = it->first + "=" + it->second;
        result[i] = new char[env_var.length() + 1];
        strcpy(result[i], env_var.c_str());
    }
    result[size] = 0; // Null-terminate the array

    return result;
}

// Executes the CGI script and returns its output
std::string CgiHandler::exec_cgi(std::string cgi_script_path)
{
    int pid;
    int pipe_stdin[2];
    int pipe_stdout[2];
    std::string new_body;

    // Create pipes for inter-process communication
    if (pipe(pipe_stdin) == -1 || pipe(pipe_stdout) == -1)
    {
        perror("pipe");
    }

    // Fork a new process
    pid = fork();

    if (pid == -1)
    {
        std::cerr << "Fork failed." << std::endl;
        return "Status: 500\r\n\r\n";
    }
    else if (!pid) // Child process
    {
        // Close the write-end of stdin and read-end of stdout pipes
        close(pipe_stdin[1]);
        close(pipe_stdout[0]);

        // Redirect stdin and stdout to pipes
        dup2(pipe_stdin[0], STDIN_FILENO);
        dup2(pipe_stdout[1], STDOUT_FILENO);

        // Execute the CGI script with environment variables
        char *argv[] = { NULL };
        execve(cgi_script_path.c_str(), argv, _env_vars_array);

        // If execve fails, print error and return a 500 status
        std::cerr << "Error: execve failed with error code: "
                  << strerror(errno) << std::endl;
        write(STDOUT_FILENO, "Status: 500\r\n\r\n", 15);
    }
    else // Parent process
    {
        // Close the read-end of stdin and write-end of stdout pipes
        close(pipe_stdin[0]);
        close(pipe_stdout[1]);

        // Write request body to stdin pipe for CGI script to read
        write(pipe_stdin[1], _cgi_request.body.c_str(), _cgi_request.body.size());
        close(pipe_stdin[1]);

        // Read the output of CGI script from stdout pipe
        char buffer[CGI_BUFSIZE] = {0};
        int ret = 1;
        while (ret > 0)
        {
            memset(buffer, 0, CGI_BUFSIZE);
            ret = read(pipe_stdout[0], buffer, CGI_BUFSIZE - 1);
            new_body += buffer;
        }
        close(pipe_stdout[0]);

        // Wait for child process to finish
        waitpid(-1, NULL, 0);
    }

    // If in child process, clean up and exit
    if (!pid)
    {
        int i = 0;
        while (_env_vars_array[i])
        {
            delete[] _env_vars_array[i];
            i++;
        }
        delete[] _env_vars_array;
        exit(0);
    }

    return new_body;
}

// Getter for _response_body
std::string CgiHandler::get_response_body()
{
    return _response_body;
}

/*
for multiple CGIs

Concurrently (using threads): 
Creating a separate thread for each request. 
Each thread would instantiate its own CgiHandler and 
execute the CGI script. 
This way, multiple CGI scripts can be run in parallel.

Issue: thread safety and resource contention
*/