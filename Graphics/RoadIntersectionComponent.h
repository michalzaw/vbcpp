#ifndef ROADINTERSECTIONCOMPONENT_H_INCLUDED
#define ROADINTERSECTIONCOMPONENT_H_INCLUDED


#include <vector>

#include "../Scene/Component.h"


class RoadObject;
class RStaticModel;


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
		static const unsigned int DEFAULT_BUFFER_SIZE = 1024 * 1024; // ~ 4600 vertices

		std::vector<RoadConnectedToIntersection> _roads;

		float _length;
		float _width;
		float _arc;
		int _quality;

		bool _needRebuildIntersectionModel;
		bool _needRebuildConnectedRoad;

		RStaticModel* _generatedModel;

		void createDebugPolygonComponent(const std::vector<std::vector<glm::vec3>>& pointsOnRoadAxis, const std::vector<std::vector<glm::vec3>>& bezierCurves);

	public:
		RoadIntersectionComponent();
		virtual ~RoadIntersectionComponent();

		void connectRoad(RoadObject* roadObject, int connectionPointInRoadIndex);
		void disconnectRoad(RoadObject* roadObject, int connectionPointInRoadIndex);

		const std::vector<RoadConnectedToIntersection>& getConnectedRoads();

		void setLength(float length);
		void setWidth(float width);
		void setArc(float arc);
		void setQuality(int quality);
		float getLength();
		float getWidth();
		float getArc();
		int getQuality();

		void createPolygon();

		void needRebuild();
		void needRebuildConnectedRoad();

		void changedTransform() override;
		void update(float deltaTime) override;

};


#endif // ROADINTERSECTIONCOMPONENT_H_INCLUDED
