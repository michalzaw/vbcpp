#ifndef LOCALIZATIONSYSTEM_H_INCLUDED
#define LOCALIZATIONSYSTEM_H_INCLUDED


#include <unordered_map>
#include <vector>
#include <regex>


class LocalizationSystem final
{
	private:
		static const std::regex FILE_NAME_PATTERN;
		static const std::string FILE_NAME_PREFIX;
		static const std::string FILE_NAME_EXTENSION;

		std::string _textsPath;
		std::string _defaultLanguage;
		std::vector<std::string> _availableLanguages;

		std::string _language;

		std::unordered_map<std::string, std::string> _texts;

		void findAvailableLanguages();
		void loadTextsFromFile();

	public:
		LocalizationSystem();
		~LocalizationSystem();

		static LocalizationSystem& getGlobalInstance();

		void initialize(const std::string& textsPath, const std::string& defaultLanguage);

		std::string getDefaultLanguage();
		const std::vector<std::string>& getAvailableLanguages();

		void setLanguage(const std::string& language);
		std::string getLanguage();

		std::string getText(const std::string& id);

};


#define GET_TEXT(textId) LocalizationSystem::getGlobalInstance().getText(#textId)
#define GET_CUSTOM_TEXT(localizationSystem, textId) localizationSystem.getText(#textId)


#endif // LOCALIZATIONSYSTEM_H_INCLUDED
