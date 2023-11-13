#ifndef CLIENTSIDE_HPP
# define CLIENTSIDE_HPP

#include "Webserv.hpp"
#include "HttpRequest.hpp"
#include "Response.hpp"
#include "ServerConfig.hpp"

//                      [**** ClientSide ****]
// - Stores Client Side info: socket, address, request, response, server
// - Client Side also stores Server object it's connected to

class ClientSide
{
    private:
        int                 _client_socket;
        struct sockaddr_in  _client_addr;
        time_t              _last_request_time;
    
    public:
        ClientSide();
        ClientSide(const ClientSide &copy);
        ClientSide(ServerConfig &);
        ClientSide &operator=(const ClientSide &rhs);
        ~ClientSide();

        const int                   &getClientSocket() const;
        const struct sockaddr_in    &getClientAddr() const;
        const HttpRequest           &getHttpRequest() const;
        const time_t                &getLastRequestTime() const;

        void                        setClientSocket(int &);
        void                        setClientAddr(struct sockaddr_in &);
        void                        setServer(ServerConfig &);
        void                        buildResponse();
        void                        updateTimer();

        void                        clearClientSide();
        Response                    response;
        HttpRequest                 httprequest;
        ServerConfig                serverconfig;
};

#endif