#ifndef LOGGER2_H_INCLUDED
#define LOGGER2_H_INCLUDED


#include <string>
#include <fstream>

#include "Strings.h"


#define VB_ENABLE_LOGGER


enum LogLevel
{
    LL_DEBUG,
    LL_INFO,
    LL_WARNING,
    LL_ERROR,
    LL_DISABLED,

    LOG_LEVELS_COUNT
};

LogLevel getLogLevelFromString(const std::string& name);


class Logger2 final
{
    private:
        static LogLevel _logLevel;
        static bool _consoleOutput;
        static bool _fileOutput;

        static std::ofstream* _logFile;

        static inline tm* getTime()
        {
            const time_t now = time(0);
            return localtime(&now);
        }

    public:
        static void init(LogLevel logLevel, bool consoleOutput, bool fileOutput, const std::string& logFileName = "");
        static void destroy();

        static void setLogLevel(LogLevel newLogLevel);
        static LogLevel getLogLevel();

        static void logMessage(LogLevel level, const char* fileName, unsigned int line, const std::string& message);

};

#ifdef VB_ENABLE_LOGGER
#define LOG_DEBUG(message) Logger2::logMessage(LL_DEBUG, __FILE__, __LINE__, message)
#define LOG_INFO(message) Logger2::logMessage(LL_INFO, __FILE__, __LINE__, message)
#define LOG_WARNING(message) Logger2::logMessage(LL_WARNING, __FILE__, __LINE__, message)
#define LOG_ERROR(message) Logger2::logMessage(LL_ERROR, __FILE__, __LINE__, message)
#else
#define LOG_DEBUG(message)
#define LOG_INFO(message)
#define LOG_WARNING(message)
#define LOG_ERROR(message)
#endif


namespace LoggerUtils
{
    template <typename TYPE>
    inline std::string variableToString(const std::string& variableName, const TYPE& variableValue)
    {
        return variableName + "=" + Strings::toString(variableValue);
    }

    inline std::string variableToString(const std::string& variableName, const std::string& variableValue)
    {
        return variableName + "=" + variableValue;
    }
}

#ifdef VB_ENABLE_LOGGER
#define LOG_VARIABLE(variable) LoggerUtils::variableToString(#variable, variable)
#else
#define LOG_VARIABLE(variable) variable
#endif


#endif // LOGGER2_H_INCLUDED
