#ifndef CROSSROADCOMPONENT_H_INCLUDED
#define CROSSROADCOMPONENT_H_INCLUDED

#include <vector>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class RoadObject;


struct CrossroadConnectionPoint
{
	glm::vec3 position;
	glm::vec3 direction;
	RoadObject* connectedRoad;

	CrossroadConnectionPoint()
		: position(0.0f, 0.0f, 0.0f), direction(1.0f, 0.0f, 0.0f),
		connectedRoad(nullptr)
	{

	}
};


class CrossroadComponent : public Component
{
	private:
		std::vector<CrossroadConnectionPoint> _connectionPoints;

	public:
		CrossroadComponent(std::vector<CrossroadConnectionPoint>& connectionPoints)
			: Component(CT_CROSSROAD),
			_connectionPoints(connectionPoints)
		{

		}

		void connectRoad(int connectionPointIndex, RoadObject* roadObject)
		{
			if (connectionPointIndex >= _connectionPoints.size())
				return;

			_connectionPoints[connectionPointIndex].connectedRoad = roadObject;
		}

		int getConnectionsCount()
		{
			return _connectionPoints.size();
		}

		CrossroadConnectionPoint& getConnectionPoint(int index)
		{
			return _connectionPoints[index];
		}

};


#endif // CROSSROADCOMPONENT_H_INCLUDED
