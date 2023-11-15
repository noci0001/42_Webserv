#include "../include/Webserv.hpp"
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
    _chunked_length = 0;
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
        if (!strcmp(rest, ".."))
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
                    std::cout << "Method Error: Request_Line: Character:" << chara << std::endl;
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
                    std::cout << "Method Error: Request_Line_Post_Put: Character:" << chara << std::endl;
                    return ;
                }
                _method_index++;
                _state = Request_Line_Method;
                break ;
            }
            case Request_Line_Method:
            {
                if (chara == _method_str[_method][_method_index])
                    _method_index++;
                else
                {
                    _error_code = 501;
                    std::cout << "Method Error: Request_Line_Method: Character:" << chara << std::endl;
                    return ;
                }
                if ((size_t) _method_index == _method_str[_method].length())
                    _state = Request_Line_First_Space;
                break ;
            }
            case Request_Line_First_Space:
            {
                if (chara != ' ')
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_First_Space: Bad Character" << std::endl;
                    return ;
                }
                _state = Request_Line_URI_Path_Slash;
                continue ;
            }
            case Request_Line_URI_Path_Slash:
            {
                if (chara == '/')
                {
                    _state = Request_Line_URI_Path;
                    _storage_buffer.clear();
                }
                else
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_URI_Path_Slash: Bad Character" << std::endl;
                    return ;
                }
                break ;
            }
            case Request_Line_URI_Path:
            {
                if (chara == ' ')
                {
                    _state = Request_Line_Version;
                    _path.append(_storage_buffer);
                    _storage_buffer.clear();
                    continue ;
                }
                else if (chara == '?')
                {
                    _state = Request_Line_URI_Query;
                    _path.append(_storage_buffer);
                    _storage_buffer.clear();
                    continue ;
                }
                else if (chara == '#')
                {
                    _state = Request_Line_URI_Fragment;
                    _path.append(_storage_buffer);
                    _storage_buffer.clear();
                    continue ;
                }
                else if (!allowedUriChar(chara))
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_URI_Path: Bad Character" << std::endl;
                    return ;
                }
                else if (i > MAX_URI_LENGTH)
                {
                    _error_code = 414;
                    std::cout << "Method Error: Request_Line_URI_Path: URI too long" << std::endl;
                    return ;
                }
                break ;
            }
            case Request_Line_URI_Query:
            {
                if (chara == ' ')
                {
                    _state = Request_Line_Version;
                    _query.append(_storage_buffer);
                    _storage_buffer.clear();
                    continue ;
                }
                else if (chara == '#')
                {
                    _state = Request_Line_URI_Fragment;
                    _query.append(_storage_buffer);
                    _storage_buffer.clear();
                    continue ;
                }
                else if (!allowedUriChar(chara))
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_URI_Query: Bad Character" << std::endl;
                    return ;
                }
                else if (i > MAX_URI_LENGTH)
                {
                    _error_code = 414;
                    std::cout << "Method Error: Request_Line_URI_Query: URI too long" << std::endl;
                    return ;
                }
                break ;
            }
            case Request_Line_URI_Fragment:
            {
                if (chara == ' ')
                {
                    _state = Request_Line_Version;
                    _fragment.append(_storage_buffer);
                    _storage_buffer.clear();
                    continue ;
                }
                else if (!allowedUriChar(chara))
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_URI_Fragment: Bad Character" << std::endl;
                    return ;
                }
                else if (i > MAX_URI_LENGTH)
                {
                    _error_code = 414;
                    std::cout << "Method Error: Request_Line_URI_Fragment: URI too long" << std::endl;
                    return ;
                }
                break ;
            }
            case Request_Line_Version:
            {
                if (checkUriPosition(_path))
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_Version: HTTP-Version before root" << std::endl;
                    return ;
                }
                if (chara != 'H')
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_Version: Bad Character" << std::endl;
                    return ;
                }
                _state = Request_Line_HT;
                break ;
            }
            case Request_Line_HT:
            {
                if (chara != 'T')
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_HT: Bad Character" << std::endl;
                    return ;
                }
                _state = Request_Line_HTT;
                break ;
            }
            case Request_Line_HTT:
            {
                if (chara != 'T')
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_HTT: Bad Character" << std::endl;
                    return ;
                }
                _state = Request_Line_HTTP;
                break ;
            }
            case Request_Line_HTTP:
            {
                if (chara != 'P')
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_HTTP: Bad Character" << std::endl;
                    return ;
                }
                _state = Request_Line_HTTP_Slash;
                break ;
            }
            case Request_Line_HTTP_Slash:
            {
                if (chara != '/')
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_HTTP_Slash: Bad Character" << std::endl;
                    return ;
                }
                _state = Request_Line_Major;
                break ;
            }
            case Request_Line_Major:
            {
                if (!isdigit(chara))
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_Major: Bad Character" << std::endl;
                    return ;
                }
                _version_major = chara;
                _state = Request_Line_Dot;
                break ;
            }
            case Request_Line_Dot:
            {
                if (chara != '.')
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_Dot: Bad Character" << std::endl;
                    return ;
                }
                _state = Request_Line_Minor;
                break ;
            }
            case Request_Line_Minor:
            {
                if (!isdigit(chara))
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_Minor: Bad Character" << std::endl;
                    return ;
                }
                _version_minor = chara;
                _state = Request_Line_EOL;
                break ;
            }
            case Request_Line_EOL:
            {
                if (chara != '\r')
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_EOL: Bad Character" << std::endl;
                    return ;
                }
                _state = Request_Line_NL;
                break ;
            }
            case Request_Line_NL:
            {
                if (chara != '\n')
                {
                    _error_code = 400;
                    std::cout << "Method Error: Request_Line_NL: Bad Character" << std::endl;
                    return ;
                }
                _state = Field_Name_Begin;
                _storage_buffer.clear();
                continue ;
            }
            case Field_Name_Begin:
            {
                if (chara == '\r')
                    _state = Field_Name_End;
                else if (isFieldnameChar(chara))
                    _state = Field_Name;
                else
                {
                    _error_code = 400;
                    std::cout << "Method Error: Field_Name_Begin: Bad Character" << std::endl;
                    return ;
                }
                break ;
            }
            case Field_Name_End:
            {
                if (chara == '\n')
                {
                    _storage_buffer.clear();
                    _fields_done_f = true;
                    _handleHeaders();
                    if (_body_f == true)
                    {
                        if (_chunked_f == true)
                            _state = Chunked_Lenght_Start;
                        else
                            _state = Message_Body;
                    }
                    else
                        _state = Http_Done;
                    continue ;
                }
                else
                {
                    _error_code = 400;
                    std::cout << "Method Error: Field_Name_End: Bad Character" << std::endl;
                    return ;
                }
                break ;
            }
            case Field_Name:
            {
                if (chara == ':')
                {
                    _storage_key = _storage_buffer;
                    _storage_buffer.clear();
                    _state = Field_Value_Begin;
                    continue ;
                }
                else if (!isFieldnameChar(chara))
                {
                    _error_code = 400;
                    std::cout << "Method Error: Field_Name: Bad Character" << std::endl;
                    return ;
                }
                break ;
            }
            case Field_Value_Begin:
            {
                if (chara == '\r')
                {
                    setHeader(_storage_key, _storage_buffer);
                    _storage_key.clear();
                    _storage_buffer.clear();
                    _state = Field_Value_End;
                    continue ;
                }
                break ;
            }
            case Field_Value_End:
            {
                if (chara == '\n')
                {
                    _state = Field_Name_Begin;
                    continue ;
                }
                else
                {
                    _error_code = 400;
                    std::cout << "Method Error: Field_Value_End: Bad Character" << std::endl;
                    return ;
                }
                break ;
            }
            case Chunked_Lenght_Start:
            {
                if (isxdigit(chara) == 0)
                {
                    _error_code = 400;
                    std::cout << "Method Error: Chunked_Length_Start: Not hexadecimal" << std::endl;
                    return ;
                }
                ss.str("");
                ss.clear();
                ss << chara;
                ss >> std::hex >> _chunked_length;
                if (_chunked_length == 0)
                    _state = Chunked_End_EOL;
                else
                    _state = Chunked_Lenght;
                continue ;
            }
            case Chunked_Lenght:
            {
                if (isxdigit(chara) != 0)
                {
                    int temp_length = 0;
                    ss.str("");
                    ss.clear();
                    ss << chara;
                    ss >> std::hex >> temp_length;
                    _chunked_length *= 16;
                    _chunked_length += temp_length;
                }
                else if (chara == '\r')
                    _state = Chunked_Lenght_NL;
                else
                    _state = Chunked_Ignore;
                continue ;
            }
            case Chunked_Lenght_EOL:
            {
                if (chara == '\r')
                    _state = Chunked_Lenght_NL;
                else
                {
                    _error_code = 400;
                    std::cout << "Method Error: Chunked_Length_EOL: Bad character" << std::endl;
                    return ;
                }
                continue ;
            }
            case Chunked_Lenght_NL:
            {
                if (chara == '\n')
                {
                    if (_chunked_length == 0)
                        _state = Chunked_End_EOL;
                    else
                        _state = Chunked_Data;
                }
                else
                {
                    _error_code = 400;
                    std::cout << "Method Error: Chunked_Length_NL: Bad character" << std::endl;
                    return ;
                }
                continue ;
            }
            case Chunked_Ignore:
            {
                if (chara == '\r')
                    _state = Chunked_Lenght_EOL;
                continue ;
            }
            case Chunked_Data:
            {
                _body.push_back(chara);
                --_chunked_length;
                if (_chunked_length == 0)
                    _state = Chunked_Data_EOL;
                continue ;
            }
            case Chunked_Data_EOL:
            {
                if (chara == '\r')
                    _state = Chunked_Data_NL;
                else
                {
                    _error_code = 400;
                    std::cout << "Method Error: Chunked_Data_EOL: Bad character" << std::endl;
                    return ;
                }
                continue ;
            }
            case Chunked_Data_NL:
            {
                if (chara == '\n')
                    _state = Chunked_Lenght_Start;
                else
                {
                    _error_code = 400;
                    std::cout << "Method Error: Chunked_Data_NL: Bad character" << std::endl;
                    return ;
                }
                continue ;
            }
            case Chunked_End_EOL:
            {
                if (chara == '\r')
                    _state = Chunked_End_NL;
                else
                {
                    _error_code = 400;
                    std::cout << "Method Error: Chunked_End_EOL: Bad character" << std::endl;
                    return ;
                }
                continue ;
            }
            case Chunked_End_NL:
            {
                if (chara != '\n')
                {
                    _error_code = 400;
                    std::cout << "Method Error: Chunked_End_NL: Bad character" << std::endl;
                    return ;
                }
                _body_done_f = true;
                _state = Http_Done;
                continue ;
            }
            case Message_Body:
            {
                if (_body.size() < _body_size)
                    _body.push_back(chara);
                if (_body.size() == _body_size)
                {
                    _body_done_f = true;
                    _state = Http_Done;
                }
                break ;
            }
            case Http_Done:
            {
                return ;
            }
        }
        _storage_buffer += chara;
    }
    if (_state == Http_Done)
        _body_str.append((char*)_body.data(), _body.size());
}

bool    HttpRequest::httpCompleted()
{
    return (_state == Http_Done);
}

//[**** Getter Functions ****]

HttpMethod  &HttpRequest::getMethod()
{
    return _method;
}

std::string &HttpRequest::getPath()
{
    return _path;
}

std::string &HttpRequest::getQuery()
{
    return _query;
}

std::string &HttpRequest::getFragment()
{
    return _fragment;
}

std::string &HttpRequest::getHeader(std::string const &key)
{
    return _headers[key];
}

const std::map<std::string, std::string> &HttpRequest::getHeaders() const
{
    return (this->_headers);
}

std::string HttpRequest::getMethodStr()
{
    return _method_str[_method];
}

std::string &HttpRequest::getBody()
{
    return _body_str;
}

std::string HttpRequest::getServerName()
{
    return (this->_server_name);
}

bool    HttpRequest::getMultiformFlag()
{
    return (this->_multiform_f);
}

std::string &HttpRequest::getBoundary()
{
    return (this->_boundary);
}

//[**** Setter Functions ****]

void    HttpRequest::setBody(std::string body)
{
    _body.clear();
    _body.insert(_body.begin(), body.begin(), body.end());
    _body_str = body;
}

void    HttpRequest::setMethod(HttpMethod &method)
{
    _method = method;
}

void    HttpRequest::setHeader(std::string &key, std::string &value)
{
    trimHttp(value);
    toLower(key);
    _headers[key] = value;
}

void    HttpRequest::setMaxBodySize(size_t size)
{
    _max_body_size = size;
}

void    HttpRequest::printRequest()
{
    std::cout << _method_str[_method] + " " + _path + "?" + _query + "#" + _fragment
        + " " + "HTTP/" << _version_major << "." << _version_minor << std::endl;
    for (std::map<std::string, std::string>::iterator ito = _headers.begin();
        ito != _headers.end(); ++ito)
    {
        std::cout << ito->first << ":" << ito->second << std::endl;
    }
    for (std::vector<u_int8_t>::iterator ito = _body.begin(); ito != _body.end(); ++ito)
    {
        std::cout << *ito;
    }
    std::cout << std::endl << "-----BODY END-----" << std::endl;
    std::cout << "BODY FLAG = " << _body_f << " BODY DONE FLAG = " << _body_done_f <<
        "FIELD FLAG = " << _fields_done_f << std::endl;
}

void    HttpRequest::_handleHeaders()
{
    std::stringstream   ss;
    if (_headers.count("content-length"))
    {
        _body_f = true;
        ss << _headers["content-length"];
        ss >> _body_size;
    }
    if (_headers.count("transfer-encoding"))
    {
        if (_headers["transfer-encoding"].find_first_of("chunked") != std::string::npos)
            _chunked_f = true;
        _body_f = true;
    }
    if (_headers.count("host"))
    {
        size_t pos = _headers["host"].find_first_of(":");
        _server_name = _headers["host"].substr(0, pos);
    }
    if (_headers.count("content-type") && _headers["content-type"].find("multipart/form-data") != std::string::npos)
    {
        size_t pos = _headers["content-type"].find("boundary=", 0);
        if (pos != std::string::npos)
            this->_boundary = _headers["content-type"].substr(pos + 9, _headers["content-type"].size());
        this->_multiform_f = true;
    }
}

short   HttpRequest::errorCode()
{
    return (this->_error_code);
}

//Resets the request object to default values for new requests
void    HttpRequest::clear()
{
    _path.clear();
    _error_code = 0;
    _query.clear();
    _fragment.clear();
    _method = NONE;
    _method_index = 1;
    _state = Request_Line;
    _body_size = 0;
    _chunked_length = 0x0;
    _storage_buffer.clear();
    _body_str = "";
    _storage_key.clear();
    _headers.clear();
    _server_name.clear();
    _body.clear();
    _boundary.clear();
    _fields_done_f = false;
    _body_f = false;
    _body_done_f = false;
    _completed_f = false;
    _chunked_f = false;
    _multiform_f = false;
}

bool    HttpRequest::keepAlive()
{
    if (_headers.count("connection"))
    {
        if (_headers["connection"].find("close", 0) != std::string::npos)
            return false;
    }
    return true;
}

void    HttpRequest::cutRequestBody(int bytes)
{
    _body_str = _body_str.substr(bytes);
}