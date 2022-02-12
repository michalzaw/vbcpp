#ifndef ROADOBJECT_H_INCLUDED
#define ROADOBJECT_H_INCLUDED


#include "RenderObject.h"
#include "Roads.h"
#include "RRoadProfile.h"


class CrossroadComponent;
class RoadIntersectionComponent;


enum class RoadType
{
	LINES_AND_ARC,
	BEZIER_CURVES,
	POINTS
};


struct RoadConnectionPoint
{
	// connection with crossroad component
	CrossroadComponent* crossroadComponent;
	int index;

	// connection with road intersection component
	RoadIntersectionComponent* roadIntersectionComponent;

	RoadConnectionPoint()
		: crossroadComponent(nullptr), index(0), roadIntersectionComponent(nullptr)
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
		std::vector<RoadConnectionPointData*> _connectionPointsData;

		float _marginBegin;
		float _marginEnd;

		// fields for BEZIER_CURVES type
		std::vector<glm::vec3> _curvePoints;

		void cutCurvePointsToIntersection(std::vector<glm::vec3>& curvePoints);
		bool isConnectionExist(int index);

	public:
		RoadObject(RoadType roadType, RRoadProfile*_roadProfile, const std::vector<glm::vec3>& points, const std::vector<RoadSegment>& segments, bool buildModelAfterCreate);
		RoadObject(RRoadProfile* _roadProfile, const std::vector<glm::vec3>& points, bool buildModelAfterCreate); // BEZIER_CURVES
		virtual ~RoadObject();

		void buildModel(bool reuseExistingModel = true);
		void buildModelLinesAndArcMode(std::vector<RoadConnectionPointData*>& connectionPointsData);
		void buildModelBezierCurvesMode(std::vector<RoadConnectionPointData*>& connectionPointsData);
		void buildModelPoints(std::vector<glm::vec3>& curvePoints, std::vector<RoadConnectionPointData*>& connectionPointsData);

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
		void setConnectionPointWithRoadIntersection(int index, RoadIntersectionComponent* roadIntersectionComponent);
		RoadConnectionPoint& getConnectionPoint(int index);

		void setCustomConnectionPointData(int index, const glm::vec3 pos, const glm::vec3 dir);

		void setMarginBegin(float marginBegin);
		void setMarginEnd(float marginEnd);
		float getMarginBegin();
		float getMarginEnd();

		void setConnectedPointPosition(int connectionPointIndex);
		void resetConnectedPointPositions();

		const std::vector<glm::vec3>& getCurvePoints();

};


#endif // ROADOBJECT_H_INCLUDED
