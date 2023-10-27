#include "../include/HttpRequest.hpp"

HttpRequest::HttpRequest()
{
    _method_str[::GET]      = "GET";
    _method_str[::HEAD]     = "HEAD";
    _method_str[::POST]     = "POST";
    _method_str[::PUT]      = "PUT";
    _method_str[::DELETE]   = "DELETE";
    _path = "";
    _query = "";
    _fragment = "";
    _body_str = "";
    _error_code = 0;
    _chunked_size = 0;
    _method = NONE;
    _method_index = 1;
    _state = Request_Line;
    _fields_done_f = false;
    _body_f = false;
    _body_done_f = false;
    _chunked_f = false;
    _body_size = 0;
    _storage_buffer = "";
    _storage_key = "";
    _multiform_f = false;
    _boundary = "";
}

HttpRequest::~HttpRequest() {}

bool    checkUriPosition(std::string path)
{
    std::string temp(path);
    char *rest = strtok((char*)temp.c_str(), "/");
    int position = 0;
    while (rest != NULL)
    {
        if (!strcmp(rest, "..."))
            position--;
        else
            position++;
        if (position < 0)
            return 1;
        rest = strtok(NULL, "/");
    }
    return 0;
}

// Checks if character is allowed in URI.
// This is specified in RFC 3986:
// - Alpanumeric: A-Z a-z 0-9
// - Unreserved: - _ . ~
// - Reserved: ! * ' ( ) ; : @ & = + $ , / ? % # [ ]
bool    allowedUriChar(u_int8_t chara)
{
    if ((chara >= '#' && chara <= ';') || (chara >= '?' && chara <= '[')
        || (chara >= 'a' && chara <= 'z') || chara == '!' || chara == '=' || chara == ']'
        || chara == '_' || chara == '~')
        return true;
    return false;
}

// Checks if character is allowed in field name.
// This is specified in RFC 7230:
// - ! # $ % & ' * + - . ^ _ ` | ~ A-Z a-z 0-9
bool    isFieldnameChar(u_int8_t chara)
{
    if (chara == '!' || (chara >= '#' && chara <= '\'') || chara == '*' || chara == '+' || chara == '-'
        || chara == '.' || (chara >= 'A' && chara <= 'Z') || (chara >= 'a' && chara <= 'z')
        || (chara >= '0' && chara <= '9') || (chara >= '^' && chara <= '`') || chara == '|')
        return true;
    return false; 
}

// Trims leading and trailing whitespace from string.
void    trimHttp(std::string &str)
{
    static const char*  space = " \t";
    str.erase(0, str.find_first_not_of(space));
    str.erase(str.find_last_not_of(space) + 1);
}

void    toLower(std::string &str)
{
    for (size_t i = 0; i < str.length(); ++i)
        str[i] = std::tolower(str[i]);
}

void HttpRequest::feed(char* data, size_t size)
{
    u_int8_t    chara;
    static std::stringstream    ss;

    for (size_t i = 0; i < size; ++i)
    {
        chara = data[i];
        switch (_state)
        {
            case Request_Line:
            {
                if (chara == 'G')
                    _method = GET;
                else if (chara == 'P')
                {
                    _state = Request_Line_Post_Put;
                    break ;
                }
                else if (chara == 'D')
                    _method = DELETE;
                else if (chara == 'H')
                    _method = HEAD;
                else
                {
                    _error_code = 501;
                    std::cout << "Method Error: Request_Line: Charachter:" << chara << std::endl;
                    return ;
                }
                _state = Request_Line_Method;
                break ;
            }
            case Request_Line_Post_Put:
            {
                if (chara == 'O')
                    _method = POST;
                else if (chara == 'U')
                    _method = PUT;
                else
                {
                    _error_code = 501;
                    std::cout << "Method Error: Request_Line_Post_Put: Charachter:" << chara << std::endl;
                    return ;
                }
                _method_index++;
                _state = Request_Line_Method;
                break ;
            }
        }
    }
}