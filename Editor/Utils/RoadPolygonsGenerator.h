#ifndef ROADPOLYGONSGENERATOR_H_INCLUDED
#define ROADPOLYGONSGENERATOR_H_INCLUDED


#include <vector>


class RoadObject;
class SceneManager;

// todo: remove (unused)
namespace RoadPolygonsGenerator
{
	void generatePolygon(const std::vector<RoadObject*>& roads, SceneManager* sceneManager);
}


#endif // ROADPOLYGONSGENERATOR_H_INCLUDED
