#include "FilesHelper.h"

#include <algorithm>
#include <fstream>

#ifdef WIN32
#include <windows.h>
#else
#include <dirent.h>
#endif


#include "Logger2.h"


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
		LOG_WARNING("Open directory failed: " + path + "!");
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
		LOG_WARNING("Directory: " + path + "is empty!");
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


std::string FilesHelper::getFileNameFromPath(std::string path)
{
	auto lastSlashPosition = path.find_last_of("/\\");

	return path.substr(lastSlashPosition + 1);
}


std::string FilesHelper::getFileExtension(std::string fileName)
{
	auto lastDotPosition = fileName.find_last_of('.');

	return fileName.substr(lastDotPosition + 1);
}


std::string FilesHelper::getPathToDirectoryFromFileName(std::string fileName)
{
	auto lastSlashPosition = fileName.find_last_of("/\\");

	return fileName.substr(0, lastSlashPosition + 1);
}


std::string FilesHelper::getRelativePathToDir(std::string filePath, std::string dirPath)
{
	std::string dirPathFromFilePath = filePath.substr(0, dirPath.size());
	if (dirPath != dirPathFromFilePath)
		LOG_ERROR("Invalid dirPath! Cannot find dirPath: " + dirPath + " in path: " + filePath);

	return filePath.substr(dirPath.size());
}


std::string FilesHelper::getRelativePathToDirInGameDir(std::string filePath, std::string dirPath)
{
	std::replace(filePath.begin(), filePath.end(), '\\', '/');
	std::replace(dirPath.begin(), dirPath.end(), '\\', '/');

	std::size_t pos = filePath.rfind(dirPath);
	if (pos != std::string::npos)
		return filePath.substr(pos + dirPath.size());
	else
		return filePath;
}

bool FilesHelper::isInPathSubdir(std::string filePath, std::string dirPath)
{
	std::replace(filePath.begin(), filePath.end(), '\\', '/');
	std::replace(dirPath.begin(), dirPath.end(), '\\', '/');

	std::size_t pos = filePath.rfind(dirPath);
	return pos != std::string::npos;
}


void FilesHelper::copyFile(std::string from, std::string to)
{
	std::ifstream sourceFile(from, std::ios::binary);
	std::ofstream destinationFile(to, std::ios::binary);

	destinationFile << sourceFile.rdbuf();
}
