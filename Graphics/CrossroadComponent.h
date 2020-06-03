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
		CrossroadComponent(std::vector<CrossroadConnectionPoint>& connectionPoints)
			: Component(CT_CROSSROAD),
			_connectionPoints(connectionPoints)
		{

		}

		void connectRoad(int connectionPointIndex, RoadObject* roadObject)
		{
			if (connectionPointIndex >= _connectionPoints.size() && roadObject != nullptr)
				return;

			_connectionPoints[connectionPointIndex].connectedRoads.push_back(roadObject);
		}

		void disconnectRoad(int connectionPointIndex, RoadObject* roadObject)
		{
			if (connectionPointIndex >= _connectionPoints.size() && roadObject != nullptr)
				return;

			for (std::list<RoadObject*>::iterator i = _connectionPoints[connectionPointIndex].connectedRoads.begin();
				 i != _connectionPoints[connectionPointIndex].connectedRoads.end();
				 ++i)
			{
				if ((*i) == roadObject)
				{
					i = _connectionPoints[connectionPointIndex].connectedRoads.erase(i);
					break;
				}
			}
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
