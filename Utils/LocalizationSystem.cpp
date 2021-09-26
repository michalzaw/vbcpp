#include "LocalizationSystem.h"

#include <memory>

#include "FilesHelper.h"
#include "Helpers.hpp"
#include "Logger.h"

#include "tinyxml2.h"
using namespace tinyxml2;


const std::regex LocalizationSystem::FILE_NAME_PATTERN = std::regex("(texts\-)([a-z]{2})(\.xml)");
const std::string LocalizationSystem::FILE_NAME_PREFIX = "texts-";
const std::string LocalizationSystem::FILE_NAME_EXTENSION = ".xml";

static std::unique_ptr<LocalizationSystem> localizationSystemGlobalInstance;


LocalizationSystem::LocalizationSystem()
	: _textsPath(""), _defaultLanguage(""), _language("")
{

}


LocalizationSystem::~LocalizationSystem()
{

}


void LocalizationSystem::findAvailableLanguages()
{
	LOG_DEBUG("Loading available languages from " + _textsPath);

	std::vector<std::string> languagesFilesNames = FilesHelper::getFilesList(_textsPath);
	for (int i = 0; i < languagesFilesNames.size(); ++i)
	{
		std::string fileName = FilesHelper::getFileNameFromPath(languagesFilesNames[i]);
		std::smatch match;
		if (std::regex_match(fileName, match, FILE_NAME_PATTERN))
		{
			_availableLanguages.push_back(match[2]);
			LOG_DEBUG(match[2]);
		}
	}
}


void LocalizationSystem::loadTextsFromFile()
{
	const std::string fileName = _textsPath + FILE_NAME_PREFIX + _language + FILE_NAME_EXTENSION;

	XMLDocument doc;
	XMLError result = doc.LoadFile(fileName.c_str());
	if (result != XML_SUCCESS)
	{
		LOG_ERROR("Cannot read xml file: " + fileName + "! Result: " + Strings::toString((int)result));
		return;
	}

	XMLElement* rootElement = doc.FirstChildElement("Texts");
	if (rootElement == nullptr)
	{
		LOG_ERROR("Texts element not found!");
		return;
	}

	for (XMLElement* textElement = rootElement->FirstChildElement("Text");
		 textElement != nullptr;
		 textElement = textElement->NextSiblingElement("Text"))
	{
		_texts[textElement->Attribute("name")] = textElement->GetText();
	}
}


LocalizationSystem& LocalizationSystem::getGlobalInstance()
{
	if (!localizationSystemGlobalInstance)
	{
		localizationSystemGlobalInstance = std::unique_ptr<LocalizationSystem>(new LocalizationSystem);
	}

	return *localizationSystemGlobalInstance;
}


void LocalizationSystem::initialize(const std::string& textsPath, const std::string& defaultLanguage)
{
	LOG_INFO("Created localization system, path: " + textsPath);

	_textsPath = textsPath;
	_defaultLanguage = defaultLanguage;

	findAvailableLanguages();
}


const std::vector<std::string>& LocalizationSystem::getAvailableLanguages()
{
	return _availableLanguages;
}


void LocalizationSystem::setLanguage(const std::string& language)
{
	if (isVectorContains(_availableLanguages, language))
	{
		LOG_INFO("Selected language: " + language);
		_language = language;
	}
	else
	{
		LOG_WARNING("Unsupported language: " + language + ". Use default language: " + _defaultLanguage);
		_language = _defaultLanguage;
	}

	loadTextsFromFile();
}


std::string LocalizationSystem::getText(const std::string& id)
{
	const auto text = _texts.find(id);
	if (text != _texts.end())
	{
		return text->second;
	}

	LOG_ERROR("Cannot find text for id: " + id);

	return "";
}
