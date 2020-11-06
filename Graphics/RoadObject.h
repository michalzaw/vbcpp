#ifndef ROADOBJECT_H_INCLUDED
#define ROADOBJECT_H_INCLUDED


#include "RenderObject.h"
#include "Roads.h"
#include "RRoadProfile.h"


class CrossroadComponent;


enum class RoadType
{
	LINES_AND_ARC,
	BEZIER_CURVES
};


struct RoadConnectionPoint
{
	CrossroadComponent* crossroadComponent;
	int index;

	RoadConnectionPoint()
		: crossroadComponent(nullptr), index(0)
	{

	}
};


class RoadObject : public RenderObject
{
	private:
		RoadType _roadType;

		RRoadProfile* _roadProfile;
		std::vector<glm::vec3> _points;
		std::vector<RoadSegment> _segments;
		std::vector<RoadConnectionPoint> _connectionPoints;

	public:
		RoadObject(RoadType roadType, RRoadProfile*_roadProfile, std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments, bool buildModelAfterCreate);
		virtual ~RoadObject();

		void buildModel(bool reuseExistingModel = true);

		RoadType getRoadType();
		RRoadProfile* getRoadProfile();
		std::vector<glm::vec3>& getPoints();
		std::vector<RoadSegment>& getSegments();

		void setRoadProfile(RRoadProfile* roadProfile);
		void setSegments(std::vector<RoadSegment> segments);

		void addPoint(glm::vec3 position);
		void deletePoint(unsigned int index);
		void setPointPostion(int index, glm::vec3 newPosition);

		void setConnectionPoint(int index, CrossroadComponent* crossroadComponent, int indexInCrossroad = 0);
		RoadConnectionPoint& getConnectionPoint(int index);

		void setConnectedPointPosition(int connectionPointIndex);
		void resetConnectedPointPositions();

};


#endif // ROADOBJECT_H_INCLUDED
