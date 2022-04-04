#include "PathComponent.h"


PathComponent::PathComponent()
	: Component(CT_PATH)
{

}


std::vector<glm::vec3>& PathComponent::getControlPoints()
{
	return _controlPoints;
}


std::vector<glm::vec3>& PathComponent::getCurvePoints()
{
	return _curvePoints;
}


void PathComponent::recalculate()
{
	for (int i = 0; i < _curvePoints.size() - 1; ++i)
	{
		glm::vec3 dir = glm::normalize(_curvePoints[i + 1] - _curvePoints[i]);
		glm::vec3 right = glm::cross(dir, glm::vec3(0, 1, 0));

		_curvePoints[i] += glm::vec3(1.5) * right;
	}
}


void PathComponent::update(float deltaTime)
{

}
