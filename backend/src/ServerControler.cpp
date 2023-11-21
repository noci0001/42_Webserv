#include "../include/Webserv.hpp"
#include "../include/ServerControler.hpp"
#include <sys/wait.h>

ServerControler::ServerControler()
{}

ServerControler::~ServerControler(){}

// [**** ServerControler ****]
// This will start all servers on ports specified
// in the configuration file.
void    ServerControler::startServer(std::vector<ServerConfig> serverconfig)
{
    std::cout << std::endl;
    ConsoleLog::logMessage(YELLOW, CONSOLE_OUTPUT, "Starting server...");
	_servers = serverconfig;
    char   buffer[INET_ADDRSTRLEN];
    bool    serverStart;
	std::vector<ServerConfig>::iterator ito;
    for (ito = _servers.begin(); ito != _servers.end(); ++ito)
    {
        serverStart = false;
		std::vector<ServerConfig>::iterator ito2;
		for (ito2 = _servers.begin(); ito2 != ito; ++ito2)
        {
            if (ito2->getHost() == ito->getHost() && ito2->getPort() == ito->getPort())
            {
                ito->setFdListen(ito2->getFdListen());
                serverStart = true;
            }
        }
        if (!serverStart)
            ito->startServer();
        ConsoleLog::logMessage(GREEN, CONSOLE_OUTPUT, "Created Server: ServerName(%s) | Host(%s) | Port(%d)", ito->getServerName().c_str(),
            		inet_ntop(AF_INET, &ito->getHost(), buffer, INET_ADDRSTRLEN), ito->getPort());
    }
}

// This runs the main loop of the server. It goes through all fds from 0 until
// the max fd in the set is found
// - checks the fds returned from select():
//      if server fd is found -> accept connection
//      if client fd is read -> read message from client
//      if client fd is write:
//          * If CGI response and Body not was not sent to CGI child process -> Send request body to CGI child process
//          * If CGI response and Body was sent to CGI child process -> Read the output from CGI child process
//          * If normal response -> Send response to client
// - Sockets for servers and clients will be added to _receive_fd_pool at first,
//   afterwards, when request if fully parsed, socket to be moved to _write_fd_pool
void    ServerControler::runServers()
{
    fd_set  receive_fd_pool;
    fd_set  write_fd_pool;
    int     select_return;

    _max_fd = 0;
    initSets();
    struct timeval timeout;
    while (true)
    {
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
        receive_fd_pool = _receive_fd_pool;
        write_fd_pool = _write_fd_pool;

        if ((select_return = select(_max_fd + 1, &receive_fd_pool, &write_fd_pool, NULL, &timeout)) < 0)
        {
            ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "Webserv: select failed: %s\t Closing Webserv", strerror(errno));
            exit(1);
            continue ;
        }
        for (int i = 0; i <= _max_fd; ++i)
        {
            if (FD_ISSET(i, &receive_fd_pool) && _map_servers.count(i))
                acceptConnection(_map_servers.find(i)->second);
            else if (FD_ISSET(i, &receive_fd_pool) && _map_clients.count(i))
                readRequest(i, _map_clients[i]);
             else if (FD_ISSET(i, &write_fd_pool) && _map_clients.count(i))
            {
                int cgi_state = _map_clients[i].response.getCgiState();
				if (cgi_state == 1 && FD_ISSET(_map_clients[i].response._cgiHandler.pipe_in[1], &write_fd_pool))
					sendBodyCgi(_map_clients[i], _map_clients[i].response._cgiHandler);
				else if (cgi_state == 1 && FD_ISSET(_map_clients[i].response._cgiHandler.pipe_out[0], &receive_fd_pool))
					readCgiResponse(_map_clients[i], _map_clients[i].response._cgiHandler);
				else if ((cgi_state == 0 || cgi_state == 2) && FD_ISSET(i, &write_fd_pool))
					sendResponse(i, _map_clients[i]);
            }
        }
        checkTimeout();
    }
}

// Checks time in seconds since last activity from client.
// If more than CONNECTION_TIMEOUT -> close connection.
void    ServerControler::checkTimeout()
{
    for (std::map<int, ClientSide>::iterator ito = _map_clients.begin(); ito != _map_clients.end(); ++ito)
    {
        if (time(NULL) - ito->second.getLastRequestTime() > CONNECTION_TIMEOUT)
        {
            ConsoleLog::logMessage(YELLOW, CONSOLE_OUTPUT, "Client %d timed out -> Closing connection.", ito->first);
            closeConnection(ito->first);
            return ;
        }
    }
}

// Initializes the receive and write fd sets.
// Adds all server listening sockets to _receive_fd_pool.
void    ServerControler::initSets()
{
    FD_ZERO(&_receive_fd_pool);
    FD_ZERO(&_write_fd_pool);

    // adds server sockets to _receive_fd_pool
	std::vector<ServerConfig>::iterator ito;
    for (ito = _servers.begin(); ito != _servers.end(); ++ito)
    {
		const ServerConfig &server = *ito;
        // calls on listen() and fcntl() to set server socket to non-blocking
        if (listen(ito->getFdListen(), 1024) == -1)
        {
            ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "Webserv: listen failed: %s\t Closing Webserv", strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (fcntl(ito->getFdListen(), F_SETFL, O_NONBLOCK) < 0)
        {
            ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "Webserv: fcntl failed: %s\t Closing Webserv", strerror(errno));
            exit(EXIT_FAILURE);
        }
        addToSets(ito->getFdListen(), _receive_fd_pool);
        _map_servers.insert(std::make_pair(ito->getFdListen(), server));
    }
    _max_fd = _servers.back().getFdListen();
}

// Accepts new connections incoming.
// Creates a new Client object and adds it to _map_clients.
// Adds Client socket to _receive_fd_pool.
void    ServerControler::acceptConnection(ServerConfig &server)
{
    struct  sockaddr_in client_address;
    long    client_address_size = sizeof(client_address);
    int     client_socket;
    ClientSide  client_new(server);
    char    buffer[INET_ADDRSTRLEN];

    if ((client_socket = accept(server.getFdListen(), (struct sockaddr *)&client_address,
        (socklen_t*) &client_address_size)) == -1)
    {
        ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "Webserv: accept failed: %s\t Closing Webserv", strerror(errno));
        return ;
    }
    ConsoleLog::logMessage(YELLOW, CONSOLE_OUTPUT, "New connection accepted from %s, socket assigned %d",
        inet_ntop(AF_INET, &client_address, buffer, INET_ADDRSTRLEN), client_socket);
    
    addToSets(client_socket, _receive_fd_pool);

    if (fcntl(client_socket, F_SETFL, O_NONBLOCK) < 0)
    {
        ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "Webserv: fcntl failed: %s\t Closing Webserv", strerror(errno));
        removeFromSets(client_socket, _receive_fd_pool);
        close(client_socket);
        return ;
    }
    client_new.setClientSocket(client_socket);
    if (_map_clients.count(client_socket) != 0)
        _map_clients.erase(client_socket);
    _map_clients.insert(std::make_pair(client_socket, client_new));
}

// Closes connection from fd.
// Removes client from _map_clients.
void    ServerControler::closeConnection(const int i)
{
    if (FD_ISSET(i, &_write_fd_pool))
        removeFromSets(i, _write_fd_pool);
    if (FD_ISSET(i, &_receive_fd_pool))
        removeFromSets(i, _receive_fd_pool);
    close(i);
    _map_clients.erase(i);
}

void    ServerControler::sendResponse(const int &i, ClientSide &client)
{
    int         sent_bytes;
    std::string response = client.response.getResponse();
    if (response.length() >= MESSAGE_BUFFER_SIZE)
        sent_bytes = write(i, response.c_str(), MESSAGE_BUFFER_SIZE);
    else
        sent_bytes = write(i, response.c_str(), response.length());
    
    if (sent_bytes < 0)
    {
        ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "Webserv: sendResponse(): write failed: %s", strerror(errno));
        closeConnection(i);
    }
    else if (sent_bytes == 0 || (size_t)sent_bytes == response.length())
    {
        ConsoleLog::logMessage(ORANGE, CONSOLE_OUTPUT, "Response sent to socket %d, Status=[%d]",
            i, client.response.getStatusCode());
        if (!client.httprequest.keepAlive() || client.httprequest.errorCode() || client.response.getCgiState())
        {
            ConsoleLog::logMessage(YELLOW, CONSOLE_OUTPUT, "Client %d: Closing connection", i);
            closeConnection(i);
        }
        else
        {
            removeFromSets(i, _write_fd_pool);
            addToSets(i, _receive_fd_pool);
            client.clearClientSide();
        }
    }
    else
    {
        client.updateTimer();
        client.response.cutResponse(sent_bytes);
    }
}

void    ServerControler::assignServer(ClientSide &client)
{
    for (std::vector<ServerConfig>::iterator ito = _servers.begin(); ito != _servers.end(); ++ito)
    {
        if (client.serverconfig.getHost() == ito->getHost() &&
            client.serverconfig.getPort() == ito->getPort() &&
            client.httprequest.getServerName() == ito->getServerName())
        {
            client.setServer(*ito);
            return ;
        }
    }
}

void    ServerControler::readRequest(const int &i, ClientSide &client)
{
    char    buffer[MESSAGE_BUFFER_SIZE];
    int     read_bytes = 0;
    read_bytes = read(i, buffer, MESSAGE_BUFFER_SIZE);
    if (read_bytes == 0)
    {
        ConsoleLog::logMessage(YELLOW, CONSOLE_OUTPUT, "Webserv: Client %d disconnected", i);
        closeConnection(i);
        return ;
    }
    else if (read_bytes < 0)
    {
        ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "Webserv: readRequest(): read failed: %s", strerror(errno));
        closeConnection(i);
        return ;
    }
    else if (read_bytes != 0)
    {
        client.updateTimer();
        client.httprequest.feed(buffer, read_bytes);
        memset(buffer, 0, sizeof(buffer));
    }

    if (client.httprequest.httpCompleted() || client.httprequest.errorCode())
    {
        assignServer(client);
        ConsoleLog::logMessage(GREEN, CONSOLE_OUTPUT, "Request parsed from socket %d, method=[%s], URI=[%s]",
            i, client.httprequest.getMethodStr().c_str(), client.httprequest.getPath().c_str());
        client.buildResponse();
        if (client.response.getCgiState())
        {
			handleRequestBody(client);
			addToSets(client.response._cgiHandler.pipe_in[1], _write_fd_pool);
			addToSets(client.response._cgiHandler.pipe_out[0], _receive_fd_pool);
        }
        removeFromSets(i, _receive_fd_pool);
        addToSets(i, _write_fd_pool);
    }
}

void	ServerControler::handleRequestBody(ClientSide &client)
{
	if (client.httprequest.getBody().length() == 0)
	{
		std::string temp;
		std::fstream file;(client.response._cgiHandler.getCgiPath().c_str());
		std::stringstream ss;
		ss << file.rdbuf();
		temp = ss.str();
		client.httprequest.setBody(temp);
	}
}

void	ServerControler::sendBodyCgi(ClientSide &client, CgiHandler &cgi)
{
	int sent_bytes;
	std::string &request_body = client.httprequest.getBody();

	if (request_body.length() == 0)
		sent_bytes = 0;
	else if (request_body.length() >= MESSAGE_BUFFER_SIZE)
		sent_bytes = write(cgi.pipe_in[1], request_body.c_str(), MESSAGE_BUFFER_SIZE);
	else
		sent_bytes = write(cgi.pipe_in[1], request_body.c_str(), request_body.length());
	if (sent_bytes < 0)
	{
		ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "Webserv: sendBodyCgi(): write failed: %s", strerror(errno));
		removeFromSets(cgi.pipe_in[1], _write_fd_pool);
		close(cgi.pipe_in[1]);
		close(cgi.pipe_out[1]);
		client.response.setErrorResponse(500);
	}
	else if (sent_bytes == 0 || (size_t)sent_bytes == request_body.length())
	{
		removeFromSets(cgi.pipe_in[1], _write_fd_pool);
		close(cgi.pipe_in[1]);
		close(cgi.pipe_out[1]);
	}
	else
	{
		client.updateTimer();
		request_body = request_body.substr(sent_bytes);
	}
}

void	ServerControler::readCgiResponse(ClientSide &client, CgiHandler &cgi)
{
	char	buffer[MESSAGE_BUFFER_SIZE * 2];
	int		read_bytes = 0;
	read_bytes = read(cgi.pipe_out[0], buffer, MESSAGE_BUFFER_SIZE * 2);

	if (read_bytes == 0)
	{
		removeFromSets(cgi.pipe_out[0], _receive_fd_pool);
		close(cgi.pipe_in[0]);
		close(cgi.pipe_out[0]);
		int status;
		waitpid(cgi.getCgiPid(), &status, 0);
		if (WEXITSTATUS(status) != 0)
			client.response.setErrorResponse(502);
		client.response.setCgiState(2);
		if (client.response._responseContent.find("HTTP/1.1") == std::string::npos)
			client.response._responseContent.insert(0, "HTTP/1.1 200 OK\r\n");
		return ;
	}
	else if (read_bytes < 0)
	{
		ConsoleLog::logMessage(RED, CONSOLE_OUTPUT, "Webserv: readCgiResponse(): read failed: %s", strerror(errno));
		removeFromSets(cgi.pipe_out[0], _receive_fd_pool);
		close(cgi.pipe_in[0]);
		close(cgi.pipe_out[0]);
		client.response.setCgiState(2);
		client.response.setErrorResponse(500);
		return ;
	}
	else
	{
		client.updateTimer();
		client.response._responseContent.append(buffer, read_bytes);
		memset(buffer, 0, sizeof(buffer));
	}
}

void    ServerControler::addToSets(const int i, fd_set &set)
{
    FD_SET(i, &set);
    if (i > _max_fd)
        _max_fd = i;
}

void    ServerControler::removeFromSets(const int i, fd_set &set)
{
    FD_CLR(i, &set);
    if (i == _max_fd)
        _max_fd--;
}