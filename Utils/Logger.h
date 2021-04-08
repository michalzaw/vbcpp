#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED


#include <string>


class Logger
{
    private:
        static constexpr char* LEVEL_INFO = "[INFO]: ";
        static constexpr char* LEVEL_WARNING = "[WARNING]: ";
        static constexpr char* LEVEL_ERROR = "[ERROR]: ";

        static void logMessage(const std::string& level, const std::string& message)
        {
            printf((level + message + "\n").c_str());
        }

    public:
        static void info(const std::string& message)
        {
            logMessage(LEVEL_INFO, message);
        }

        static void warning(const std::string& message)
        {
            logMessage(LEVEL_WARNING, message);
        }

        static void error(const std::string& message)
        {
            logMessage(LEVEL_ERROR, message);
        }

};


#endif // LOGGER_H_INCLUDED
