#include "BusRoutesLoader.h"

#include "../../Utils/Logger.h"
#include "../../Utils/ResourceDescriptionUtils.h"
#include "../../Utils/XmlUtils.h"

#include "../../Utils/tinyxml2.h"
using namespace tinyxml2;


namespace BusRoutesLoader
{
	void loadRoute(XMLElement* routeElement, std::unordered_map<std::string, BusRoute>& routes)
	{
		std::string id = XmlUtils::getAttributeString(routeElement, "id");

		if (routes.count(id) > 0)
		{
			LOG_WARNING("Skip duplcated route id=" + id);
			return;
		}

		routes[id] = {};
		BusRoute& route = routes[id];

		route.id = id;
		route.direction = XmlUtils::getAttributeString(routeElement, "direction");
		route.type = static_cast<BusRouteDisplayType>(XmlUtils::getAttributeInt(routeElement, "type"));
		route.head = XmlUtils::getAttributeStringOptional(routeElement, "head");
		route.line1 = XmlUtils::getAttributeStringOptional(routeElement, "line1");
		route.line2 = XmlUtils::getAttributeStringOptional(routeElement, "line2");
	}


	void loadLine(XMLElement* lineElement, BusRoutes* routes)
	{
		std::string id = XmlUtils::getAttributeString(lineElement, "id");
		std::string number = XmlUtils::getAttributeString(lineElement, "number");

		if (routes->lines.count(id) > 0)
		{
			LOG_WARNING("Skip duplcated line id=" + id);
			return;
		}

		routes->lines[id] = BusLine(id, number);

		for (XMLElement* routeElement = lineElement->FirstChildElement("Route");
			 routeElement != nullptr;
			 routeElement = routeElement->NextSiblingElement("Route"))
		{
			loadRoute(routeElement, routes->lines[id].routes);
		}
	}


	void loadLines(XMLElement* routesElement, BusRoutes* routes)
	{
		for (XMLElement* lineElement = routesElement->FirstChildElement("Line");
			 lineElement != nullptr;
			 lineElement = lineElement->NextSiblingElement("Line"))
		{
			loadLine(lineElement, routes);
		}
	}


	BusRoutes* loadRoutes(const std::string& dirPath, const std::string& fileName)
	{
		XMLDocument doc;
		XMLError result = doc.LoadFile((dirPath + fileName).c_str());
		if (result != XML_SUCCESS)
		{
			LOG_ERROR("Cannot read xml file: " + fileName + "! Result: " + Strings::toString((int)result));
		}

		// Search for main element - Object
		XMLElement* routesElement = doc.FirstChildElement("Routes");
		if (routesElement == nullptr)
		{
			LOG_ERROR("Routes element not found!");
			return nullptr;
		}

		XMLElement* routesDesc = routesElement->FirstChildElement("Description");
		if (routesDesc == nullptr)
		{
			LOG_ERROR("Description element not found!");
			return nullptr;
		}

		BusRoutes* routes = new BusRoutes;
		routes->fileName = fileName;

		ResourceDescriptionUtils::loadResourceDescription(routesDesc, routes->description);

		LOG_DEBUG("*** ROUTES DATA ***");
		LOG_DEBUG("Author: " + routes->description.author);
		LOG_DEBUG("Name: " + routes->description.name);
		LOG_DEBUG("Comment: " + routes->description.comment);


		loadLines(routesElement, routes);


		return routes;
	}
}