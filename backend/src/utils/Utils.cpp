#include "../../include/Webserv.hpp"

int ft_stoi(std::string str)
{
    std::stringstream strs(str);
    if (str.length() > 10)
        throw std::exception();
    for (size_t i = 0; i < str.length(); ++i)
    {
        if (!isdigit(str[i]))
            throw std::exception();
    }
    int res;
    strs >> res;
    return res;
}