#ifndef STATUSCODE_HPP
#define STATUSCODE_HPP

#include <map>
#include <string>

class StatusCode {
public:
    StatusCode();
    ~StatusCode();
    std::string getStatusCode( int code );

private:
    std::map<int, std::string> _statusCode;
};

#endif