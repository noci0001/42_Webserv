#ifndef SERVERCONTROLER_HPP
# define SERVERCONTROLER_HPP

#include "Webserv.hpp"

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
        void    startServers(std::vector<ServerConfig>);
        void    runServers();

    private:
        std::vector<ServerConfig>   _servers;
        std::map<int, ServerConfig> _map_servers;
        std::map<int, Client>       _map_clients;
        fd_set                      _receive_fd_pool;
        fd_set                      _write_fd_pool;
        int                         _max_fd;

        void    acceptConnection(ServerConfig &);
        void    checkTimeout();
        void    initSets();
        void    readRequest(const int &, Client &);
        void    handleRequestBody(Client &);
        void    sendResponse(const int &, Client &);
        //void    sendCgiBody(Client &, CgiHandler &);
        //void    readCgiResponse(Client &, CgiHandler &);
        void    closeConnection(const int &);
        void    assignServer(Client &);
        void    addToSets(const int, fd_set &);
        void    removeFromSets(const int, fd_set &);
};

#endif