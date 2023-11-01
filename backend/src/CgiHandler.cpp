#include "../include/Webserv.hpp"

// Define a buffer size for CGI responses
#define CGI_BUFSIZE 8192

// Default constructor
CgiHandler::CgiHandler() {}

// Parameterized constructor
CgiHandler::CgiHandler(Request _cgi_request) : _cgi_request(_cgi_request)
{
    // Create a temporary map of environment variables for CGI execution
    std::map<std::string, std::string> tmp_env_vars = _create_env_vars();

    // Convert the map of environment variables into an array
    _env_vars_array = _create_env_vars_array(tmp_env_vars);

    // Initialize response body to empty string
    _response_body = "";
}

// Destructor: Cleans up dynamically allocated memory for environment variables array
CgiHandler::~CgiHandler()
{
    int i = 0;
    while (_env_vars_array[i]) {
        delete[] _env_vars_array[i];
        i++;
    }
    delete[] _env_vars_array;
}

// Executes the CGI script and stores the response in _response_body
void CgiHandler::run()
{
    _response_body = exec_cgi(_cgi_request.cgi_path);
}

// Creates a map of environment variables based on the provided CGI request
std::map<std::string, std::string> CgiHandler::_create_env_vars()
{
    std::map<std::string, std::string> tmp_env_vars;

    // These environment variables are standard for executing CGI scripts
    // and provide the script with information about the request
    // and server environment
    // More information can be found in the CGI specification
    tmp_env_vars["AUTH_TYPE"]         = _cgi_request.authorization;
    tmp_env_vars["REDIRECT_STATUS"]   = "200";
    tmp_env_vars["GATEWAY_INTERFACE"] = "CGI/1.1";
    tmp_env_vars["SCRIPT_NAME"]       = _cgi_request.cgi_path;
    tmp_env_vars["SCRIPT_FILENAME"]   = _cgi_request.cgi_path;
    tmp_env_vars["REQUEST_METHOD"]    = _cgi_request.method;
    tmp_env_vars["CONTENT_LENGTH"]    = _cgi_request.body; // This might need conversion to char* or length of body
    tmp_env_vars["CONTENT_TYPE"]      = _cgi_request.content_type;
    tmp_env_vars["PATH_INFO"]         = _cgi_request.cgi_path;
    tmp_env_vars["PATH_TRANSLATED"]   = _cgi_request.cgi_path;
    tmp_env_vars["QUERY_STRING"]      = _cgi_request.query;
    tmp_env_vars["REMOTEaddr"]        = _cgi_request.port;
    tmp_env_vars["REMOTE_IDENT"]      = _cgi_request.authorization;
    tmp_env_vars["REMOTE_USER"]       = _cgi_request.authorization;
    tmp_env_vars["REQUEST_URI"]       = _cgi_request.cgi_path + _cgi_request.query;
    tmp_env_vars["SERVER_NAME"]       = _cgi_request.host;
    tmp_env_vars["SERVER_PROTOCOL"]   = "HTTP/1.1";
    tmp_env_vars["SERVER_SOFTWARE"]   = "Webserv/1.0";

    return tmp_env_vars;
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