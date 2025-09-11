#ifndef BUSROUTES_H_INCLUDED
#define BUSROUTES_H_INCLUDED


#include <string>
#include <unordered_map>

#include "../../Graphics/DisplayComponent.h"

#include "../../Utils/ResourceDescription.h"


enum BusRouteDisplayType
{
	RDT_ONE_LINE,
	RDT_TWO_LINE,
	RDT_TWO_LINE_FIRST_BIG,
	RDT_TWO_LINE_SECOND_BIG
	
};


struct BusRoute final
{
	std::string id;
	std::string direction;

	BusRouteDisplayType type;

	std::string head;
	std::string line1;
	std::string line2;

};


struct BusLine final
{
	std::string id;
	std::string number;

	std::unordered_map<std::string, BusRoute> routes;

	BusLine(const std::string& id, const std::string& number)
		: id(id), number(number)
	{

	}

	BusLine()
		: id(""), number("")
	{

	}

};


struct BusRoutes final
{
	ResourceDescription description;

	std::unordered_map<std::string, BusLine> lines;

};


#endif // BUSROUTES_H_INCLUDED
