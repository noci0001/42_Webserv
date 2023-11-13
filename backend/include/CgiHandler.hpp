#ifndef CGIHANDLER_HPP
#define CGIHANDLER_HPP

#include "Webserv.hpp"
#include "HttpRequest.hpp"


class HttpRequest;

class CgiHandler {

private:
    // Attributes
    //HttpRequest     _cgi_request;
    //std::string     _response_body; // new body to be sent to the client
    char            **_envi_vars_array;
    char            **_argv;
    int             _exit_return;
    std::map<std::string, std::string>  _envi;
    std::string     _cgi_path;
    pid_t           _cgi_pid;

    // Methods
    char **
    _create_env_vars_array( std::map<std::string, std::string> env_vars );
    std::map<std::string, std::string> _create_env_vars();

public:
    int     pipe_in[2];
    int     pipe_out[2];
    // Constructors and destructors
    CgiHandler();
    CgiHandler(std::string path);
    ~CgiHandler();
    CgiHandler(CgiHandler const &other);

    // Operator Overload Assignment
    CgiHandler &operator=(CgiHandler const &rhs);

    // Methods

    void createEnvi(HttpRequest &httprequest, const std::vector<Location>::iterator ito_loca);
    void createEnviVars(HttpRequest &httprequest, const std::vector<Location>::iterator ito_loca);
    void run(short &error_code);
    void sendBodyHeader(int &pipe_out, int &fd, std::string &);
    void fixHeader(std::string &fix_header);
    void clearCgiHandler();

	int findStart(const std::string path, const std::string delimiter);
	std::string getPathInfo(std::string &path, std::vector<std::string> cgi_extensions);
	std::string decodeHttpRequest(std::string &path);
    
    void setCgiPid(pid_t cgi_pid);
    void setCgiPath(const std::string &cgi_path);

    const std::map<std::string, std::string> &getEnvi() const;
    const pid_t &getCgiPid() const;
    const std::string &getCgiPath() const;

    std::string get_response_body();
    std::string exec_cgi( std::string cgi_script_path );
};

#endif

// A CGI path is the file location of a Common Gateway Interface (CGI) script on
// a web server. It is the location where the server can find the script to
// execute it. An example of a CGI path could be
// "/usr/local/cgi-bin/my_script.cgi" or
// "C:\inetpub\wwwroot\cgi-bin\my_script.pl" depending on the operating system
// and web server configuration. The path must be the full path on the file
// system, not a URL.