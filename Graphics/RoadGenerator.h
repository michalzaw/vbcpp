#ifndef ROADGENERATOR_H_INCLUDED
#define ROADGENERATOR_H_INCLUDED


#include <vector>

#include "Roads.h"


class RStaticModel;


namespace RoadGenerator
{
	glm::vec2 calculateDirection(const std::vector<glm::vec3>& points, const std::vector<RoadConnectionPointData*>& roadConnectionPoints, int currentPointIndex);

	RStaticModel* createRoadModel(const std::vector<RoadLane>& roadLanes, const std::vector<glm::vec3>& points,
								  const std::vector<RoadConnectionPointData*>& roadConnectionPoints, RStaticModel* oldModel = nullptr);
}


#endif // ROADGENERATOR_H_INCLUDED
