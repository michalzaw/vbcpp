#include "Gearbox.h"

// XML reader
#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

#include <iostream>

#include "ResourceManager.h"
#include "FilesHelper.h"
#include "Logger.h"

#include "../Game/Directories.h"

Gearbox::Gearbox(std::string filename)
: _currentGear(1) // position in gear vector container; 1 - neutral
{
    loadData(filename);
}


Gearbox::~Gearbox()
{
    _gears.clear();
}


void Gearbox::shiftUp()
{
    if (_currentGear < _gearCount - 1)
        _currentGear++;
}


void Gearbox::shiftDown()
{
    if (_currentGear > 0)
        _currentGear--;
}


void Gearbox::setNeutral()
{
    _currentGear = EGM_NEUTRAL;
}


void Gearbox::loadData(std::string filename)
{
    std::string fullPath = GameDirectories::BUS_PARTS + filename + ".xml";

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(fullPath))
		fullPath = ResourceManager::getInstance().getAlternativeResourcePath() + fullPath;
#endif // DEVELOPMENT_RESOURCES

    XMLDocument doc;
    doc.LoadFile( fullPath.c_str() );

    // Search for main element - Gearbox
    XMLElement* gbxElement = doc.FirstChildElement("Gearbox");
    if (gbxElement == nullptr) LOG_ERROR("Gearbox element not found! Is it correct gearbox file?");

    XMLElement* gbxDesc = gbxElement->FirstChildElement("Description");
    if (gbxDesc == nullptr) LOG_ERROR("Description element not found");

    // Load file description
    std::string author(gbxDesc->Attribute("author"));
    std::string model(gbxDesc->Attribute("name"));
    std::string comment(gbxDesc->Attribute("comment"));

    // Search for gear list element
    XMLElement* gearList = gbxElement->FirstChildElement("Gears");
    _gearCount = atoi(gearList->Attribute("count"));


    // Iterate through list of gears values
    XMLElement* listElement = gearList->FirstChildElement("Gear");

    while (listElement != nullptr)
    {
        float gearRatio;

        listElement->QueryFloatText(&gearRatio);
        _gears.push_back(gearRatio);

        listElement = listElement->NextSiblingElement("Gear");
    }

    // Set current default gear
    _currentGear = EGM_NEUTRAL;

    // Print debug data on screen
    LOG_INFO("*** GEARBOX DATA ***");
    LOG_INFO("Author: " + author);
    LOG_INFO("Model: " + model);
    LOG_INFO("Comment: " + comment);

    LOG_INFO("Gear count: " + Strings::toString(_gearCount));

    //for (int i = 0; i < _gears.size(); i++)
    //    std::cout << "Gear " << i << ": " << _gears[i] << std::endl;
}
