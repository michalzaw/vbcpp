#include "RScriptFile.h"

#include <filesystem>
#include <fstream>

#include "../Utils/FilesHelper.h"


RScriptFile::RScriptFile(const std::string& path, const std::string& script)
	: Resource(RT_SCRIPT, path),
	_script(script)
{

}


std::string RScriptFile::loadScriptFromFile(const std::string& fileName)
{
	if (!FilesHelper::isFileExists(fileName))
	{
		LOG_ERROR("Error while loading script file. File " + fileName + " not exist.");
		return "";
	}

	/*auto fileSize = std::filesystem::file_size(fileName);
	std::string fileContent(fileSize, '\0');

	std::ifstream inputStream(fileName);
	inputStream.read(&fileContent[0], fileSize);*/

	std::ifstream file(fileName);
	std::stringstream stringstream;
	stringstream << file.rdbuf();
	
	std::string fileContent = stringstream.str();

	LOG_DEBUG("New script loaded");
	LOG_DEBUG(fileContent);

	return fileContent;
}
