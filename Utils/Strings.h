#ifndef STRINGS_H_INCLUDED
#define STRINGS_H_INCLUDED


#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <type_traits>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>


// Convert number to string
template <typename TYPE>
std::string toString(TYPE Number)
{
	std::ostringstream Stream;
	Stream << Number;
	return Stream.str();
}

inline std::string vec2ToString(const glm::vec2& vector)
{
	std::ostringstream Stream;
	Stream << vector.x << "," << vector.y;
	return Stream.str();
}

inline std::string vec3ToString(const glm::vec3& vector)
{
	std::ostringstream Stream;
	Stream << vector.x << "," << vector.y << "," << vector.z;
	return Stream.str();
}


inline std::string vec4ToString(const glm::vec4& vector)
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


// New

namespace Strings
{
	namespace Internal
	{
		template<typename T> struct IsArithmeticTypeForToString
		{
			static bool const value =
				std::is_arithmetic<T>::value;
		};

		template<typename T> struct IsGlmTypeForToString
		{
			static bool const value =
				std::is_same<T, glm::vec2>::value ||
				std::is_same<T, glm::vec3>::value ||
				std::is_same<T, glm::vec4>::value ||
				std::is_same<T, glm::mat2>::value ||
				std::is_same<T, glm::mat3>::value ||
				std::is_same<T, glm::mat4>::value;
		};

		template<typename T> struct IsStringTypeForToString
		{
			static bool const value =
				std::is_same<T, std::string>::value;
		};

		template <typename T>
		class HasToStringMethod
		{
			typedef char one;
			struct two { char x[2]; };

			template <typename C> static one test(decltype(&C::toString));
			template <typename C> static two test(...);

		public:
			static bool const value = sizeof(test<T>(0)) == sizeof(char);
		};

		template<typename TYPE>
		std::string toString(std::enable_if_t<IsArithmeticTypeForToString<TYPE>::value, TYPE> value)
		{
			return std::to_string(value);
		}

		template<typename TYPE>
		std::string toString(const std::enable_if_t<IsGlmTypeForToString<TYPE>::value, TYPE>& value)
		{
			return glm::to_string(value);
		}

		template<typename TYPE>
		std::string toString(const std::enable_if_t<IsStringTypeForToString<TYPE>::value, TYPE>& value)
		{
			return value;
		}

		template<typename TYPE>
		std::string toString(const std::enable_if_t<HasToStringMethod<TYPE>::value, TYPE>& value)
		{
			return value.toString();
		}
	}

	// Public functions

	// Convert variable to std::string
	// Supported types:
	// - Built-in numeric types
	// - glm::vec2, glm::vec3, glm::vec4, glm::mat2, glm::mat3, glm::mat4
	// - any object with const method toString()
	template<typename TYPE>
	inline std::string toString(const TYPE& value)
	{
		return Internal::toString<TYPE>(value);
	}
}


#endif // STRINGS_H_INCLUDED
