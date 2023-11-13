#ifndef SERVERCONTROLER_HPP
# define SERVERCONTROLER_HPP

#include <sys/socket.h>
#include "Webserv.hpp"
#include "ClientSide.hpp"
#include "HttpRequest.hpp"
#include "Response.hpp"
#include "CgiHandler.hpp"

//       [**** ServerControler ****]
// This class is used to control the server.
// responsibilities:
// - start the server with the given configuration
// - establish the connection with the clients and 
//   receive/send requests/responses

class ServerControler
{
    public:
        ServerControler();
        ~ServerControler();

		void    startServer(std::vector<ServerConfig> serverconfig);
        void    runServers();

    private:
        std::vector<ServerConfig>   _servers;
        std::map<int, ServerConfig> _map_servers;
        std::map<int, ClientSide>   _map_clients;
        fd_set                      _receive_fd_pool;
        fd_set                      _write_fd_pool;
        int                         _max_fd;

        void    acceptConnection(ServerConfig &);
        void    checkTimeout();
        void    initSets();
        void    readRequest(const int &, ClientSide &);
        void    handleRequestBody(ClientSide &);
        void    sendResponse(const int &, ClientSide &);
		void	sendBodyCgi(ClientSide &client, CgiHandler &cgi);
        void    readCgiResponse(ClientSide &, CgiHandler &);
        void    closeConnection(const int);
        void    assignServer(ClientSide &);
        void    addToSets(const int, fd_set &);
        void    removeFromSets(const int, fd_set &);
};

#endif