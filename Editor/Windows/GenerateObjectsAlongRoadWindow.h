#ifndef GENERATEOBJECTSALONGROADWINDOW_H_INCLUDED
#define GENERATEOBJECTSALONGROADWINDOW_H_INCLUDED


#include <string>
#include <vector>


class RoadObject;


namespace vbEditor
{
	bool generateObjectsAlongRoadWindow(const std::vector<std::string>& availableObjects, RoadObject* roadComponent);
}


#endif // GENERATEOBJECTSALONGROADWINDOW_H_INCLUDED