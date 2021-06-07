#ifndef FILESHELPER_H_INCLUDED
#define FILESHELPER_H_INCLUDED


#include <vector>
#include <string>


class FilesHelper
{
    public:
        static bool isFileExists(std::string fileName)
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

		static bool isDirectoryExists(std::string dirPath);

        static std::vector<std::string> getDirectoriesList(std::string path);

		static std::string getFileNameFromPath(std::string path);
		static std::string getFileExtension(std::string fileName);
        static std::string getPathToDirectoryFromFileName(std::string fileName);

		static std::string getRelativePathToDir(std::string filePath, std::string dirPath);
		static std::string getRelativePathToDirInGameDir(std::string filePath, std::string dirPath);
		static bool isInPathSubdir(std::string filePath, std::string dirPath);

		static void copyFile(std::string from, std::string to);

};


#endif // FILESHELPER_H_INCLUDED
