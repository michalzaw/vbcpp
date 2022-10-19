#ifndef POLYGONGENERATOR_H_INCLUDED
#define POLYGONGENERATOR_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>

#include "Logger.h"


namespace PolygonGenerator
{
	int getPreviousPointIndex(std::vector<glm::vec3>& points, std::vector<bool>& points2, int index);


	int getNextPointIndex(std::vector<glm::vec3>& points, std::vector<bool>& points2, int index);


	bool isConvexPoint(std::vector<glm::vec3>& points, std::vector<bool>& points2, int index);


	float sign(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3);


	bool isTriangleContainsVertex(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 v);


	bool isTriangleContainsNoConcaveVertex(std::vector<glm::vec3>& points, std::vector<bool>& points2, int index);


	int findEar(std::vector<glm::vec3>& points, std::vector<bool>& points2);
}


#endif // POLYGONGENERATOR_H_INCLUDED