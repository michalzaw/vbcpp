#ifndef ROADOBJECT_H_INCLUDED
#define ROADOBJECT_H_INCLUDED


#include "RenderObject.h"
#include "Roads.h"
#include "RRoadProfile.h"


class CrossroadComponent;


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
		RRoadProfile* _roadProfile;
		std::vector<glm::vec3> _points;
		std::vector<RoadSegment> _segments;
		std::vector<RoadConnectionPoint> _connectionPoints;

	public:
		RoadObject(RRoadProfile*_roadProfile, std::vector<glm::vec3>& points, std::vector<RoadSegment>& segments, bool buildModelAfterCreate);
		virtual ~RoadObject();

		void buildModel(bool reuseExistingModel = true);

		RRoadProfile* getRoadProfile();
		std::vector<glm::vec3>& getPoints();
		std::vector<RoadSegment>& getSegments();

		void setRoadProfile(RRoadProfile* roadProfile);
		void setSegments(std::vector<RoadSegment> segments);

		void addPoint(glm::vec3 position);
		void deletePoint(unsigned int index);

		void setConnectionPoint(int index, CrossroadComponent* crossroadComponent, int indexInCrossroad = 0);
		RoadConnectionPoint& getConnectionPoint(int index);

};


#endif // ROADOBJECT_H_INCLUDED
