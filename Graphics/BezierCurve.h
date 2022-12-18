#ifndef BEZIERCURVE_H_INCLUDED
#define BEZIERCURVE_H_INCLUDED


#include <vector>
#include <functional>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class BezierCurve final : public Component
{
	private:
		std::vector<glm::vec3> _points;
		std::vector<int> _segmentsPointsCount;

		float _marginBegin;
		float _marginEnd;
		glm::vec2 _offsetFromBaseCurve;

		std::vector<glm::vec3> _curvePoints;
		bool _curvePointsIsCalculated;

		std::vector<int> _pointsWithLockedPosition;

		std::function<void()> _onPointAddedListener;
		std::function<void(int)> _onPointDeletedListener;
		std::function<void(int, const glm::vec3&)> _onPointChangedPositionListener;
		std::function<void()> _onCurveChangedListener;
		std::function<void()> _onComponentDeletedListener;

		void calculateCurvePoints();
		void cutCurvePointsToMargin(std::vector<glm::vec3>& curvePoints);
		void applyCurveOffset(std::vector<glm::vec3>& curvePoints);

	public:
		BezierCurve(const std::vector<glm::vec3>& points = {}, const std::vector<int>& segmentsPointsCount = {},
					float marginBegin = 0.0f, float marginEnd = 0.0f, const glm::vec2& offsetFromBaseCurve = glm::vec2(0.0f, 0.0f));
		~BezierCurve();

		void addPoint(const glm::vec3& position);
		void deletePoint(int index);
		void setPointPostion(int index, const glm::vec3& newPosition);

		const std::vector<glm::vec3>& getCurvePoints();
		int getSegmentPointsCount(int segmentIndex);
		int getSegmentsCount();

		void setSegmentPointsCount(int segmentIndex, int pointsCount);

		inline const std::vector<glm::vec3>& getPoints() { return _points; }
		inline float getMarginBegin() { return _marginBegin; }
		inline float getMarginEnd() { return _marginEnd; }
		inline const glm::vec2& getOffsetFromBaseCurve() { return _offsetFromBaseCurve; }

		void setMarginBegin(float marginBegin);
		void setMarginEnd(float marginEnd);
		void setOffsetFromBaseCurve(const glm::vec2& offsetFromBaseCurve);

		void lockPointPosition(int index);
		void unlockPointPosition(int index);

		BezierCurve* splitCurve(int segmentIndex, float t); // Dzieli krzywa w danym punkcie, pierwsza czesc zostaje w istniejacej instancji, druga trafia do nowego componentu
		void cutAndGetCurve(float marginBegin, float marginEnd, std::vector<glm::vec3>& outPoints, std::vector<int>& outSegmentsPointsCount); // Zwraca parametry dla nowej krzywej obcietej z obu stron (nie modyfikuje istniejacej)

		void setOnPointAddedListener(const std::function<void(void)>& onPointAddedListener);
		void setOnPointDeletedListener(const std::function<void(int)>& onPointDeletedListener);
		void setOnPointChangedPositionListener(const std::function<void(int, const glm::vec3&)>& onPointChangedPositionListener);
		void setOnCurveChangedListener(const std::function<void()>& onCurveChangedListener);
		void setOnComponentDeletedListener(const std::function<void()>& onComponentDeletedListener);

};


#endif // BEZIERCURVE_H_INCLUDED