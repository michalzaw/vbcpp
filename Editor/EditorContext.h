#ifndef EDITORCONFIG_H_INCLUDED
#define EDITORCONFIG_H_INCLUDED


#include <string>
#include <vector>


struct MapInfo
{
	std::string name;
	std::string author;
};


struct EditorContext
{
	std::vector<std::string> availableObjects;

	MapInfo currentMapInfo;
};


#endif // EDITORCONFIG_H_INCLUDED
