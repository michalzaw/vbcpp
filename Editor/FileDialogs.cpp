#include "FileDialogs.h"

#include <portable-file-dialogs.h>


namespace FileDialogs
{
	std::vector<std::string> openFile(const std::string& title)
	{
		return pfd::open_file("Choose texture...").result();
	}
};
