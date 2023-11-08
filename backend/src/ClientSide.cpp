#include "../include/Webserv.hpp"

ClientSide::ClientSide()
{
    _last_request_time = time(NULL);
}

ClientSide::~ClientSide()   {}

//[** Copy Constructor **]
ClientSide::ClientSide(const ClientSide &copy)
{
    if (this != &copy)
    {
        this->_client_socket = copy._client_socket;
        this->_client_addr = copy._client_addr;
        this->_last_request_time = copy._last_request_time;
        this->httprequest = copy.httprequest;
        //this->response = copy.response;
        this->serverconfig = copy.serverconfig;
    }
    return ;
}

ClientSide::ClientSide(ServerConfig &serverconfig)
{
    setServer(serverconfig);
    httprequest.setMaxBodySize(serverconfig.getMaxBodySizeClient());
    _last_request_time = time(NULL);
}

void    ClientSide::setClientSocket(int &client_socket)
{
    _client_socket = client_socket;
}

void    ClientSide::setClientAddr(struct sockaddr_in &client_addr)
{
    _client_addr = client_addr;
}

void    ClientSide::setServer(ServerConfig &serverconfig)
{
    //response.setServerConfig(serverconfig);
}

const int   &ClientSide::getClientSocket() const
{
    return _client_socket;
}

const struct sockaddr_in    &ClientSide::getClientAddr() const
{
    return _client_addr;
}

const HttpRequest   &ClientSide::getHttpRequest() const
{
    return httprequest;
}

const time_t    &ClientSide::getLastRequestTime() const
{
    return _last_request_time;
}

void    ClientSide::updateTimer()
{
    _last_request_time = time(NULL);
}

void    ClientSide::buildResponse()
{
    //response.setHttpRequest(this->httprequest);
    //response.buildResponse();
}

void    ClientSide::clearClientSide()
{
    //response.clear();
    httprequest.clear();
}