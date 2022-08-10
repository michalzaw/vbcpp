#ifndef LOGGERWINDOW_H_INCLUDED
#define LOGGERWINDOW_H_INCLUDED


#include <string>


enum LogLevel : int;


namespace LoggerWindow
{
	void init();
	void addLog(LogLevel level, const std::string& message);
	void drawWindow();
}


#endif // LOGGERWINDOW_H_INCLUDED