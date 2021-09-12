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
	Stream << vector.x << "," << vector.y << "," << vector.z;
	return Stream.str();
}


inline std::string vec4ToString(glm::vec4 vector)
{
	std::ostringstream Stream;
	Stream << vector.x << "," << vector.y << "," << vector.z << "," << vector.w;
	return Stream.str();
}


// Convert string to int
inline int toInt(const std::string& String)
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
inline float toFloat(const std::string& String)
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
inline bool toBool(const std::string& String)
{
    return strcmp(String.c_str(), "true") == 0;
}


inline bool toBool(const char* String)
{
    return strcmp(String, "true") == 0;
}


std::string trim(const std::string& string);


std::vector<std::string> split(const std::string& string, char delimiter);

bool startsWith(const std::string& string, const std::string& start);


namespace Utf8String
{
	unsigned int getNextUtf8Char(const std::string& string, unsigned int& offset);
}


#endif // STRINGS_H_INCLUDED
