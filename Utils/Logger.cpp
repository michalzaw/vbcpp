#include "Logger.h"

#include <iomanip>
#include <iostream>
#include <chrono>
#include <ctime>

#include "FilesHelper.h"


LogLevel Logger::_logLevel = LL_DISABLED;
bool Logger::_consoleOutput = false;
bool Logger::_fileOutput = false;
std::ofstream* Logger::_logFile = nullptr;
std::vector<std::function<void(LogLevel, const std::string&)>> Logger::_customAppenders = {};


const std::string logLevelsNames[] =
{
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    ""
};

LogLevel getLogLevelFromString(const std::string& name)
{
    for (int i = 0; i < LOG_LEVELS_COUNT; ++i)
    {
        if (logLevelsNames[i] == name)
            return static_cast<LogLevel>(i);
    }
    return LL_DISABLED;
}


void Logger::init(LogLevel logLevel, bool consoleOutput, bool fileOutput, const std::string& logFileName)
{
    _logLevel = logLevel;
    _consoleOutput = consoleOutput;
    _fileOutput = fileOutput;

    if (_fileOutput)
    {
        if (logFileName.empty())
        {
            _fileOutput = false;
            LOG_ERROR("Log fileName is empty!");
        }
        else
        {
            _logFile = new std::ofstream;
            _logFile->open(logFileName);
        }
    }

    LOG_INFO("Logger initialization complete.");
    LOG_INFO("Logger settings: logLevel=" + logLevelsNames[_logLevel] + " consoleOutput=" + toString(_consoleOutput) + " fileOutput=" + toString(_fileOutput));
}


void Logger::destroy()
{
    if (_logFile != nullptr)
    {
        _logFile->close();

        delete _logFile;
        _logFile = nullptr;
    }

    _logLevel = LL_DISABLED;
    _consoleOutput = false;
    _fileOutput = false;
    _logFile = nullptr;

}


void Logger::setLogLevel(LogLevel newLogLevel)
{
    LOG_INFO("Changed logLevel: " + logLevelsNames[_logLevel] + " -> " + logLevelsNames[newLogLevel]);
    if (newLogLevel != LL_DISABLED)
    {
        LOG_INFO("Logger is disabled");

    }

    _logLevel = newLogLevel;
}


LogLevel Logger::getLogLevel()
{
    return _logLevel;
}


void Logger::addCustomAppender(const std::function<void(LogLevel level, const std::string&)>& appender)
{
    _customAppenders.push_back(appender);
}


void Logger::logMessage(LogLevel level, const char* fileName, unsigned int line, const std::string& message)
{
    if (level < _logLevel)
    {
        return;
    }

    if (!_consoleOutput && !_fileOutput)
    {
        return;
    }

    const tm* time = getTime();
    const std::string fileNameWithoutPath = FilesHelper::getFileNameFromPath(fileName);

    std::ostringstream stream;
    stream
        << std::setfill('0') << std::right << std::setw(2) << time->tm_hour << ":" << std::setw(2) << time->tm_min << ":" << std::setw(2) << time->tm_sec << " "
        << "[" << std::setfill(' ') << std::setw(7) << logLevelsNames[level] << "]"
        << "[" << std::setw(31) << fileNameWithoutPath << ": "
        << std::left << std::setw(4) << line << "]: "
        << message;

    const std::string logLine = stream.str();

    if (_consoleOutput)
    {
        //printf("%d:%d:%d [%7s][%s: %d]: %s\n",
        //    time->tm_hour, time->tm_min, time->tm_sec,
        //    logLevelsNames[level].c_str(),
        //    fileNameWithoutPath.c_str(),
        //    line,
        //    message.c_str());
        std::cout << logLine << std::endl;
    }

    if (_fileOutput)
    {

        *_logFile << logLine << std::endl;
    }

    for (const auto& appender : _customAppenders)
    {
        appender(level, logLine);
    }
}
