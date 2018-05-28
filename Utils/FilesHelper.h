#ifndef FILESHELPER_H_INCLUDED
#define FILESHELPER_H_INCLUDED


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

            fclose(file);
            return 0;
        }

};


#endif // FILESHELPER_H_INCLUDED
