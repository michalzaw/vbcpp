#ifndef FILESHELPER_H_INCLUDED
#define FILESHELPER_H_INCLUDED


#include <vector>
#include <string>


class FilesHelper
{
    public:
        static bool isFileExists(const std::string& fileName)
        {
            FILE* file;
            file = fopen(fileName.c_str(), "r");
            if (file)
            {
                fclose(file);
                return 1;
            }

            return 0;
        }

		static bool isDirectoryExists(const std::string& dirPath);

        static std::vector<std::string> getFilesList(const std::string& path);
        static std::vector<std::string> getDirectoriesList(const std::string& path);

		static std::string getFileNameFromPath(const std::string& path);
		static std::string getFileExtension(const std::string& fileName);
        static std::string getPathToDirectoryFromFileName(const std::string& fileName);

		static std::string getRelativePathToDir(const std::string& filePath, const std::string& dirPath);
		static std::string getRelativePathToDirInGameDir(std::string filePath, std::string dirPath);
		static bool isInPathSubdir(std::string filePath, std::string dirPath);

		static void copyFile(const std::string& from, const std::string& to);

};


#endif // FILESHELPER_H_INCLUDED
