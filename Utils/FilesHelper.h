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

        static std::vector<std::string> getDirectoriesList(std::string path);

		static std::string getFileExtension(std::string fileName);

};


#endif // FILESHELPER_H_INCLUDED
