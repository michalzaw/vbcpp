#include "Strings.h"


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
