#ifndef PATHCOMPONENT_H_INCLUDED
#define PATHCOMPONENT_H_INCLUDED


#include <vector>

#include <glm/glm.hpp>

#include "../Scene/Component.h"


class PathComponent final : public Component
{
	private:
		glm::vec2 _distanceFromBaseBezierCurve;

		std::vector<glm::vec3> _baseBezierCurveControlPoints;
		std::vector<glm::vec3> _curvePoints;

		float _marginBegin;
		float _marginEnd;

		void onAttachedToScenObject() override;

	public:
		PathComponent(const std::vector<glm::vec3>& baseBezierCurveControlPoints, const glm::vec2& distanceFromBaseBezierCurve, float marginBegin = 0.0f, float marginEnd = 0.0f);

		inline const std::vector<glm::vec3>& getBaseBezierCurveControlPoints() { return _baseBezierCurveControlPoints; }
		inline const std::vector<glm::vec3>& getCurvePoints() { return _curvePoints; }
		inline float getMarginBegin() { return _marginBegin; }
		inline float getMarginEnd() { return _marginEnd; }

		inline void setMarginBegin(float marginBegin) { _marginBegin = marginBegin; }
		inline void setMarginEnd(float marginEnd) { _marginBegin = marginEnd; }

		void recalculate();

		void update(float deltaTime) override;
};


#endif // PATHCOMPONENT_H_INCLUDED
