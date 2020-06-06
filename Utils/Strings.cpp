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


bool startsWith(std::string string, std::string start)
{
	return string.rfind(start, 0) == 0;
}
