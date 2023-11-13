#ifndef CONSOLELOG_HPP
# define CONSOLELOG_HPP

#include "Webserv.hpp"

#define RESET          "\x1B[0m"
#define RED            "\x1B[31m"
#define GREEN          "\x1B[32m"
#define ORANGE         "\x1B[33m"

#define LIGHT_RED      "\x1B[91m"
#define WHITE          "\x1B[37m"
#define BLINK           "\x1b[5m"
#define YELLOW         "\x1B[33m"
#define LIGHT_BLUE     "\x1B[94m"
#define CYAN           "\x1B[36m"
#define DARK_GREY      "\x1B[90m"
#define LIGHTMAGENTA   "\x1B[95m"
#define GST             4;

enum ConsolePrio
{
    INFO,
    DEBUG,
    ERROR
};

enum Log_State
{
    ON,
    OFF
};

enum Mode
{
    CONSOLE_OUTPUT,
    FILE_OUTPUT
};

class ConsoleLog
{
    private:
        static std::map<ConsolePrio, std::string> initMap();
    
    public:
        static std::string file_name;
        static ConsolePrio prio;
        static std::map<ConsolePrio, std::string> prio_str;
        static Log_State state;

        static void setFileName(std::string);
        static void setState(Log_State);

        static void logMessage(const char *, Mode, const char*, ...);
        static void setPrio(ConsolePrio);
        static void enableFileLog();
        static std::string  getTimestamp();
};

#endif