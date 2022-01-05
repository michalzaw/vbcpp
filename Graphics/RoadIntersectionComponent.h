#ifndef ROADINTERSECTIONCOMPONENT_H_INCLUDED
#define ROADINTERSECTIONCOMPONENT_H_INCLUDED


#include <vector>

#include "../Scene/Component.h"


class RoadObject;


struct RoadConnectedToIntersection final
{
	RoadObject* road;
	int connectionPointInRoadIndex;

	RoadConnectedToIntersection(RoadObject* road, int connectionPointInRoadIndex)
		: road(road), connectionPointInRoadIndex(connectionPointInRoadIndex)
	{}
};


class RoadIntersectionComponent final : public Component
{
	private:
		std::vector<RoadConnectedToIntersection> _roads;

		float _length;
		float _width;

		std::vector<std::vector<glm::vec3>> _bezierCurves;

		bool _needRebuildConnectedRoad;

	public:
		RoadIntersectionComponent();
		virtual ~RoadIntersectionComponent();

		void connectRoad(RoadObject* roadObject, int connectionPointInRoadIndex);
		void disconnectRoad(RoadObject* roadObject, int connectionPointInRoadIndex);

		const std::vector<RoadConnectedToIntersection>& getConnectedRoads();

		void setLength(float length);
		void setWidth(float width);
		float getLength();
		float getWidth();

		void createPolygon();

		void needRebuildConnectedRoad();

		void changedTransform() override;
		void update(float deltaTime) override;

};


#endif // ROADINTERSECTIONCOMPONENT_H_INCLUDED
