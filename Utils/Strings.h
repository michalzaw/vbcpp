#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED


#include <string>
#include <sstream>
#include <cstring>
#include <vector>

#include <glm/glm.hpp>


// Convert number to string
template <typename TYPE>
std::string toString(TYPE Number)
{
	std::ostringstream Stream;
	Stream << Number;
	return Stream.str();
}


inline std::string vec3ToString(glm::vec3 vector)
{
	std::ostringstream Stream;
	Stream << "[" << vector.x << ", " << vector.x << ", " << vector.x << "]";
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


// Convert string to bool
inline bool toBool(std::string String)
{
    return strcmp(String.c_str(), "true") == 0;
}


inline bool toBool(const char* String)
{
    return strcmp(String, "true") == 0;
}


std::string trim(const std::string& string);


std::vector<std::string> split(const std::string& string, char delimiter);


#endif // STRINGS_H_INCLUDED
