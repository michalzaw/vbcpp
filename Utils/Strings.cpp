#include "Strings.h"

#include <sstream>


std::string trim(const std::string& string)
{
    size_t first = string.find_first_not_of(' ');
    if (first == std::string::npos)
    {
        return string;
    }

    size_t last = string.find_last_not_of(' ');

    return string.substr(first, (last - first + 1));
}


std::vector<std::string> split(const std::string& string, char delimiter)
{
	std::istringstream stream(string);

	std::vector<std::string> result;
	std::string s;
	while (getline(stream, s, delimiter))
	{
		result.push_back(s);
	}

	return result;
}


bool startsWith(const std::string& string, const std::string& start)
{
	return string.rfind(start, 0) == 0;
}

namespace Utf8String
{
#define VALIDATE_CONTININUATION_BYTE(byte) if (byte < 0x80 || byte >= 0xc0) return '?';

    unsigned int decodeUtf8Char(const char*& string)
    {
        if (string == nullptr)
        {
            return 0;
        }

        unsigned char char1 = *string;
        ++string;

        if (char1 >= 0x80 && char1 < 0xc0) // char1 >= 10000000 && char1 < 11000000 - bajt nie jest poczatkiem punktu kodowego. Iterujemy przez string do momentu trafienia na poczatek kolejnego punktu kodowego i zwracamy '?'
        {
            while (static_cast<unsigned int>(*string) >= 0x80 && static_cast<unsigned int>(*string) < 0xc0)
            {
                ++string;
            }
            return '?';
        }

        if (char1 < 0x80) // char1 < 10000000
        {
            return char1;
        }
        else if (char1 < 0xe0) // char1 < 11100000
        {
            unsigned char char2 = *(string++);
            VALIDATE_CONTININUATION_BYTE(char2);

            return static_cast<unsigned int>(char2 & 0x3fu | ((char1 & 0x1fu) << 6u));
        }
        else if (char1 < 0xf0) // char1 < 11110000
        {
            unsigned char char2 = *(string++);
            VALIDATE_CONTININUATION_BYTE(char2);
            unsigned char char3 = *(string++);
            VALIDATE_CONTININUATION_BYTE(char3);

            return static_cast<unsigned int>(char3 & 0x3fu | ((char2 & 0x3fu) << 6u) | ((char1 & 0xfu) << 12u));
        }
        else // if (char1 < 0xf8)
        {
            unsigned char char2 = *(string++);
            VALIDATE_CONTININUATION_BYTE(char2);
            unsigned char char3 = *(string++);
            VALIDATE_CONTININUATION_BYTE(char3);
            unsigned char char4 = *(string++);
            VALIDATE_CONTININUATION_BYTE(char4);

            return static_cast<unsigned int>(char4 & 0x3fu | ((char3 & 0x3fu) << 6u) | ((char2 & 0x3fu) << 12u) | ((char1 & 0x7u) << 18u));
        }
    }


    unsigned int getNextUtf8Char(const std::string& string, unsigned int& offset)
    {
        const char* buffer = string.c_str() + offset;
        unsigned int charValue = decodeUtf8Char(buffer);
        offset = buffer - string.c_str();

        return charValue;
    }
}
