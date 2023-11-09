#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include "Webserv.hpp"

enum HttpMethod
{
    GET,
    POST,
    DELETE,
    PUT,
    HEAD,
    NONE
};

enum HttpState
{
    Request_Line,
    Request_Line_Post_Put,
    Request_Line_Method,
    Request_Line_First_Space,
    Request_Line_URI_Path_Slash,
    Request_Line_URI_Path,
    Request_Line_URI_Query,
    Request_Line_URI_Fragment,
    Request_Line_Version,
    Request_Line_HT,
    Request_Line_HTT,
    Request_Line_HTTP,
    Request_Line_HTTP_Slash,
    Request_Line_Major,
    Request_Line_Dot,
    Request_Line_Minor,
    Request_Line_EOL,
    Request_Line_NL,
    Field_Name_Begin,
    Field_Name_End,
    Field_Name,
    Field_Value_Begin,
    Field_Value_End,
    Chunked_Lenght_Start,
    Chunked_Lenght,
    Chunked_Ignore,
    Chunked_Lenght_EOL,
    Chunked_Lenght_NL,
    Chunked_Data,
    Chunked_Data_EOL,
    Chunked_Data_NL,
    Chunked_End_EOL,
    Chunked_End_NL,
    Message_Body,
    Http_Done
};

class HttpRequest
{
    private:
        std::string                         _path;
        std::string                         _query;
        std::string                         _fragment;
        std::map<std::string, std::string>  _headers;
        std::vector<u_int8_t>               _body;
        std::string                         _body_str;
        std::string                         _boundary;
        HttpMethod                          _method;
        std::map<u_int8_t, std::string>     _method_str;
        HttpState                           _state;
        size_t                              _max_body_size;
        size_t                              _body_size;
        short                               _error_code;
        size_t                              _chunked_length;
        std::string                         _storage_buffer;
        std::string                         _storage_key;
        short                               _method_index;
        u_int8_t                            _version_major;
        u_int8_t                            _version_minor;
        std::string                         _server_name;

        // Http Request Flags
        bool                                _fields_done_f;
        bool                                _body_f;
        bool                                _body_done_f;
        bool                                _completed_f;
        bool                                _chunked_f;
        bool                                _multiform_f;

        void                                _handleHeaders();

    public:
        HttpRequest();
        ~HttpRequest();

        HttpMethod                                  &getMethod();
        std::string                                 &getPath();
        std::string                                 &getQuery();
        std::string                                 &getFragment();
        std::string                                 &getHeader(std::string const &);
        const std::map<std::string, std::string>    &getHeaders() const;
        std::string                                 getMethodStr();
        std::string                                 &getBody();
        std::string                                 getServerName();
        std::string                                 &getBoundary();
        bool                                        getMultiformFlag();

        void                                        setMethod(HttpMethod &);
        void                                        setHeader(std::string &, std::string &);
        void                                        setMaxBodySize(size_t);
        void                                        setBody(std::string name);

        void                                        feed(char *data, size_t size);
        bool                                        httpCompleted();
        void                                        printRequest();
        void                                        clear();
        short                                       errorCode();
        bool                                        keepAlive();
        void                                        cutRequestBody(int bytes);
};

#endif