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
}