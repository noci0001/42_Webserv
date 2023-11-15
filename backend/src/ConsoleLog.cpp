#include "../include/Webserv.hpp"
#include "../include/ConsoleLog.hpp"
#include <stdarg.h>

std::string ConsoleLog::file_name = "serverlog.txt";
ConsolePrio ConsoleLog::prio = ERROR;
Log_State ConsoleLog::state = ON;

std::map<ConsolePrio, std::string> ConsoleLog::prio_str = initMap();

std::map<ConsolePrio, std::string> ConsoleLog::initMap()
{
    std::map<ConsolePrio, std::string> l_map;

    l_map[INFO] = "DEBUG\t";
    l_map[DEBUG] = "INFO\t";
    l_map[ERROR] = "ERROR\t";

    return l_map;
}

void    ConsoleLog::logMessage(const char *color, Mode mode, const char* message, ...)
{
    char    output[8192];
    va_list args;
    int     len;

    if (state == ON)
    {
        va_start(args, message);
        len = vsnprintf(output, 8192, message, args);
        std::string time = getTimestamp();
        if (mode == FILE_OUTPUT)
        {
            if (mkdir("serverlogs", 0777) < 0 && errno != EEXIST)
            {
                std::cerr << "Error while mkdir(): " << strerror(errno) << std::endl;
                return ;
            }
            int fd = open(("serverlogs/" + file_name).c_str(),
                O_CREAT | O_WRONLY | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IWUSR);
            std::cout << "value of fd: " << fd << "value of errno: " << strerror(errno) << std::endl;
            write(fd, time.c_str(), time.length());
            write(fd, "   ", 3);
            write(fd, output, len);
            write(fd, "\n", 1);
            close(fd);
        }
        else if (mode == CONSOLE_OUTPUT)
        {
            std::cout << color << getTimestamp() << output << RESET << std::endl;
        }
        va_end(args);
    }
}

std::string ConsoleLog::getTimestamp()
{
    tzset();
    char date[1000];
    time_t now = time(0);
    struct tm tm = *localtime(&now);
    strftime(date, sizeof(date), "[%Y-%m-%d %H:%M:%S]  ", &tm);
    return (std::string(date));
}

void    ConsoleLog::setPrio(ConsolePrio p)
{
    ConsoleLog::prio = p;
}

void    ConsoleLog::setState(Log_State s)
{
    ConsoleLog::state = s;
}

void    ConsoleLog::setFileName(std::string name)
{
    ConsoleLog::file_name = name;
}