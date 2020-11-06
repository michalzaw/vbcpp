#ifndef CROSSROADCOMPONENT_H_INCLUDED
#define CROSSROADCOMPONENT_H_INCLUDED

#include <vector>
#include <list>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class RoadObject;


struct CrossroadConnectionPoint
{
	glm::vec3 position;
	glm::vec3 direction;
	std::list<RoadObject*> connectedRoads;

	CrossroadConnectionPoint()
		: position(0.0f, 0.0f, 0.0f), direction(1.0f, 0.0f, 0.0f)
	{

	}
};


class CrossroadComponent : public Component
{
	private:
		std::vector<CrossroadConnectionPoint> _connectionPoints;

	public:
		CrossroadComponent(std::vector<CrossroadConnectionPoint>& connectionPoints);

		void connectRoad(int connectionPointIndex, RoadObject* roadObject);
		void disconnectRoad(int connectionPointIndex, RoadObject* roadObject);

		int getConnectionsCount();

		CrossroadConnectionPoint& getConnectionPoint(int index);

		glm::vec3 getGlobalPositionOfConnectionPoint(int index);
		glm::vec3 getGlobalDirectionOfConnectionPoint(int index);

		void changedTransform() override;

};


#endif // CROSSROADCOMPONENT_H_INCLUDED
