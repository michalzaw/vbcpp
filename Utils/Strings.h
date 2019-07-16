#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED


#include <string>
#include <sstream>


// Convert number to string
template <typename TYPE>
std::string toString(TYPE Number)
{
	std::ostringstream Stream;
	Stream << Number;
	return Stream.str();
}


// Convert string to int
inline int toInt(std::string String)
{
	std::istringstream Stream(String);
	int Temp;
	Stream >> Temp;
	return Temp;
}


inline int toInt(const char* String)
{
	std::istringstream Stream(String);
	int Temp;
	Stream >> Temp;
	return Temp;
}


// Convert string to float
inline float toFloat(std::string String)
{
	std::istringstream Stream(String);
	float Temp;
	Stream >> Temp;
	return Temp;
}


inline float toFloat(const char* String)
{
	std::istringstream Stream(String);
	float Temp;
	Stream >> Temp;
	return Temp;
}


std::string trim(const std::string& string);


#endif // STRINGS_H_INCLUDED
