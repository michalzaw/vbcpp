#ifndef BEZIERCURVE_H_INCLUDED
#define BEZIERCURVE_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class BezierCurve final : public Component
{
	private:
		std::vector<glm::vec3> _points;
		std::vector<int> _segmentsPointsCount;

		float _marginBegin;
		float _marginEnd;

		std::vector<glm::vec3> _curvePoints;
		bool _curvePointsIsCalculated;

		void calculateCurvePoints(); // void RoadObject::buildModelBezierCurvesMode(std::vector<RoadConnectionPointData*>&)
		void cutCurvePointsToMargin(std::vector<glm::vec3>& curvePoints); // void RoadObject::cutCurvePointsToIntersection(std::vector<glm::vec3>&)

	public:
		BezierCurve();

		void addPoint(const glm::vec3& position); // void RoadObject::addPoint(glm::vec3);
		void deletePoint(int index); // void RoadObject::deletePoint(unsigned int);
		void setPointPostion(int index, const glm::vec3& newPosition); // void RoadObject::setPointPostion(int, glm::vec3);

		const std::vector<glm::vec3>& getCurvePoints();
		int getSegmentPointsCount(int segmentIndex);
		int getSegmentsCount();

		void setSegmentPointsCount(int segmentIndex, int pointsCount);

		inline const std::vector<glm::vec3>& getPoints() { return _points; }
		inline float getMarginBegin() { return _marginBegin; }
		inline float getMarginEnd() { return _marginEnd; }

		void setMarginBegin(float marginBegin);
		void setMarginEnd(float marginEnd);

};


#endif // BEZIERCURVE_H_INCLUDED