#include "../include/Webserv.hpp"
#include "../include/ServerControler.hpp"

ServerControler::ServerControler() {}
ServerControler::~ServerControler() {}

// [**** ServerControler ****]
// This will start all servers on ports specified
// in the configuration file.
void    ServerControler::startServers(std::vector<ServerConfig> servers)
{
    std::cout << std::endl;
    ConsoleLog::logMessage(YELLOW, CONSOLE_OUTPUT, "Starting servers...");
    _servers = servers;
    char   buffer[INET_ADDRSTRLEN];
    bool    serverStart;
    for (std::vector<ServerConfig>::iterator ito = _servers.begin(); ito != _servers.end(); ++ito)
    {
        serverStart = false;
        for (std::vector<ServerConfig>::iterator ito2 = _servers.begin(); ito2 != ito; ++ito2)
        {
            if (ito2->getHost() == ito->getHost() && ito2->getPort() == ito->getPort())
            {
                ito->setFdListen(ito2->getFdListen());
                serverStart = true;
            }
        }
        if (!serverStart)
            ito->startServer();
        ConsoleLog::logMessage(GREEN, CONSOLE_OUTPUT,
            "Created Server: ServerName(%s) | Host(%s) | Port(%d)", ito->getServerName().c_str(),
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
    while (1)
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
            /* else if (FD_ISSET(i, &write_fd_pool) && _map_clients.count(i))
            {
                // implementation of CGI
            } */
        }
        checkTimeout();
    }
}

// Checks time in seconds since last activity from client.
// If more than CONNECTION_TIMEOUT -> close connection.
void    ServerControler::checkTimeout()
{
    for (std::map<int, Client>::iterator ito = _map_clients.begin(); ito != _map_clients.end(); ++ito)
    {
        if (time(NULL) - ito->second.getLastTime() > CONNECTION_TIMEOUT)
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
    for (std::vector<ServerConfig>::iterator ito = _servers.begin(); ito != _servers.end(); ++ito)
    {
        // calls on listen() and fcntl() to set server socket to non-blocking
        if (listen(ito->getFdListen(), 512) == -1)
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
        _map_servers.insert(std::make_pair(ito->getFdListen(), *ito));
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
    Client  client_new(server);
    char    buffer[INET_ADDRSTRLEN];

    if (client_socket = accept(server.getFdListen(), (struct sockaddr *)&client_address,
        ((socklen_t*) &client_address_size)) == -1)
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
    client_new.setFd(client_socket);
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

void    ServerControler::sendResponse(const int &i, Client &client)
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
        if (client.request.keepAlive() == false || client.request.errorCode() /* || client.response.getCgiState() */)
        {
            ConsoleLog::logMessage(YELLOW, CONSOLE_OUTPUT, "Client %d: Closing connection", i);
            closeConnection(i);
        }
        else
        {
            removeFromSets(i, _write_fd_pool);
            addToSets(i, _receive_fd_pool);
            client.clearClient();
        }
    }
    else
    {
        client.updateTimestamp();
        client.response.cutResponse(sent_bytes);
    }
}

void    ServerControler::assignServer(Client &client)
{
    for (std::vector<ServerConfig>::iterator ito = _servers.begin(); ito != _servers.end(); ++ito)
    {
        if (client.server.getHost() == ito->getHost() &&
            client.server.getPort() == ito->getPort() &&
            client.request.getServerName() == ito->getServerName())
        {
            client.setServer(*ito);
            return ;
        }
    }
}

void    ServerControler::readRequest(const int &i, Client &client)
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
        client.updateTimestamp();
        client.request.feed(buffer, read_bytes);
        memset(buffer, 0, sizeof(buffer));
    }

    if (client.request.parsingComplete() || client.request.errorCode())
    {
        assignServer(client);
        ConsoleLog::logMessage(GREEN, CONSOLE_OUTPUT, "Request parsed from socket %d, method=[%s], URI=[%s]",
            i, client.request.getMethod().c_str(), client.request.getUri().c_str());
        client.buildResponse();
        /* if (client.response.getCgiState())
        {
            //CGI imlementation
        } */
        removeFromSets(i, _receive_fd_pool);
        addToSets(i, _write_fd_pool);
    }
}