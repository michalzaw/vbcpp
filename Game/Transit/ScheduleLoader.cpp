#include "ScheduleLoader.h"

#include "../../Utils/Logger.h"
#include "../../Utils/ResourceDescriptionUtils.h"
#include "../../Utils/XmlUtils.h"

#include "../../Utils/tinyxml2.h"
using namespace tinyxml2;


namespace ScheduleLoader
{
	void loadStop(XMLElement* stopElement, std::vector<ScheduleStop>& stops)
	{
		int id = XmlUtils::getAttributeInt(stopElement, "id");
		Time time = XmlUtils::getAttributeTime(stopElement, "time");

		stops.emplace_back(id, time);
	}

	void loadRoute(XMLElement* routeElement, std::vector<ScheduleRoute>& routes)
	{
		std::string direction = XmlUtils::getAttributeString(routeElement, "direction");
		Time startTime = XmlUtils::getAttributeTime(routeElement, "startTime");

		ScheduleRoute& route = routes.emplace_back(direction, startTime);

		for (XMLElement* stopElement = routeElement->FirstChildElement("Stop");
			stopElement != nullptr;
			stopElement = stopElement->NextSiblingElement("Stop"))
		{
			loadStop(stopElement, route.stops);
		}
	}


	void loadBrigade(XMLElement* brigadeElement, std::vector<ScheduleBrigade>& brigades)
	{
		std::string id = XmlUtils::getAttributeString(brigadeElement, "id");
		std::string name = XmlUtils::getAttributeString(brigadeElement, "name");

		ScheduleBrigade& brigade = brigades.emplace_back(id, name);

		for (XMLElement* routeElement = brigadeElement->FirstChildElement("Route");
			routeElement != nullptr;
			routeElement = routeElement->NextSiblingElement("Route"))
		{
			loadRoute(routeElement, brigade.routes);
		}
	}


	void loadLine(XMLElement* lineElement, Schedule* schedule)
	{
		std::string id = XmlUtils::getAttributeString(lineElement, "id");
		std::string number = XmlUtils::getAttributeStringOptional(lineElement, "number", id);

		ScheduleLine& line = schedule->lines.emplace_back(id, number);

		for (XMLElement* brigadeElement = lineElement->FirstChildElement("Brigade");
			brigadeElement != nullptr;
			brigadeElement = brigadeElement->NextSiblingElement("Brigade"))
		{
			loadBrigade(brigadeElement, line.brigades);
		}
	}


	void loadLines(XMLElement* scheduleElement, Schedule* schedule)
	{
		for (XMLElement* lineElement = scheduleElement->FirstChildElement("Line");
			lineElement != nullptr;
			lineElement = lineElement->NextSiblingElement("Line"))
		{
			loadLine(lineElement, schedule);
		}
	}


	Schedule* loadSchedule(const std::string& dirPath, const std::string& fileName)
	{
		XMLDocument doc;
		XMLError result = doc.LoadFile((dirPath + fileName).c_str());
		if (result != XML_SUCCESS)
		{
			LOG_ERROR("Cannot read xml file: " + fileName + "! Result: " + Strings::toString((int)result));
		}

		// Search for main element - Object
		XMLElement* scheduleElement = doc.FirstChildElement("Schedule");
		if (scheduleElement == nullptr)
		{
			LOG_ERROR("Schedule element not found!");
			return nullptr;
		}

		XMLElement* routesDesc = scheduleElement->FirstChildElement("Description");
		if (routesDesc == nullptr)
		{
			LOG_ERROR("Description element not found!");
			return nullptr;
		}

		Schedule* schedule = new Schedule;
		schedule->fileName = fileName;

		ResourceDescriptionUtils::loadResourceDescription(routesDesc, schedule->description);

		LOG_DEBUG("*** ROUTES DATA ***");
		LOG_DEBUG("Author: " + schedule->description.author);
		LOG_DEBUG("Name: " + schedule->description.name);
		LOG_DEBUG("Comment: " + schedule->description.comment);


		loadLines(scheduleElement, schedule);


		return schedule;
	}
}