#include "FilesHelper.h"

#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif


#include "Logger.h"


bool FilesHelper::isDirectoryExists(std::string dirPath)
{
	#ifdef WIN32
	DWORD type = GetFileAttributesA(dirPath.c_str());
	if (type == INVALID_FILE_ATTRIBUTES)
	{
		return false;
	}
	if (type & FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	}
	return false;
	#else
	DIR* dir = opendir(dirPath.c_str());
	if (dir)
	{
		closedir(dir);
		return true;
	}
	return false;
	#endif
}


std::vector<std::string> FilesHelper::getDirectoriesList(std::string path)
{
    std::vector<std::string> directories;

    #ifdef WIN32
    path += "*";

    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(path.c_str(), &findFileData);
    if (hFind == INVALID_HANDLE_VALUE) {
        Logger::warning("Open directory failed: " + path + "!");
        return directories;
    }

    do
    {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            std::string dirName = findFileData.cFileName;
            if (dirName != "." && dirName != "..")
            {
                directories.push_back(findFileData.cFileName);
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
    #else

    auto dir = opendir(path.c_str());

    if (dir == NULL)
    {
        Logger::warning("Directory: " + path + "is empty!");
        return directories;
    }

    auto entity = readdir(dir);

    while (entity != NULL)
    {
        if (entity->d_type == DT_DIR)
        {
            directories.push_back(entity->d_name);
        }

        entity = readdir(dir);
    }

    closedir(dir);

    #endif

    return directories;
}

std::string FilesHelper::getFileExtension(std::string fileName)
{
	auto lastDotPosition = fileName.find_last_of('.');

	return fileName.substr(lastDotPosition + 1);
}
