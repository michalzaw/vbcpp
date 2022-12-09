#include "AIAgent.h"

#include "PathComponent.h"

#include "../Graphics/SkeletalAnimationComponent.h"

#include "../Scene/SceneObject.h"

#include "../Utils/QuaternionUtils.h"


AIAgent::AIAgent()
	: Component(CT_AI_AGENT),
	_speed(10.0f),
	_currentPath(nullptr), _skeletalAnimationComponent(nullptr),
	_speedInAnimation(10.0f), _baseAnimationTicksPerSecond(0.0f),
	_currentPointIndex(0), _t(0.0f)
{

}


void AIAgent::onAttachedToScenObject()
{
	_skeletalAnimationComponent = dynamic_cast<SkeletalAnimationComponent*>(getSceneObject()->getComponent(CT_SKELETAL_ANIMATION));
	if (_skeletalAnimationComponent != nullptr)
	{
		glm::vec3 startPosition = _skeletalAnimationComponent->getRootBonePositionInStartFrame() * _skeletalAnimationComponent->getScale();
		glm::vec3 endPosition = _skeletalAnimationComponent->getRootBonePositionInEndFrame() * _skeletalAnimationComponent->getScale();

		float distanceFromStartToEndPosition = glm::distance(startPosition, endPosition);
		float animationDuration = _skeletalAnimationComponent->getAnimationDuration() / _skeletalAnimationComponent->getAnimationTicksPerSecond();

		_speedInAnimation = distanceFromStartToEndPosition / animationDuration;

		_baseAnimationTicksPerSecond = _skeletalAnimationComponent->getAnimationTicksPerSecond();

		setSpeed(_speed);
	}
}


void AIAgent::reset()
{
	_currentPointIndex = 0;
	_t = 0.0f;

	const auto& nextPaths = _currentPath->getNextPaths();
	if (nextPaths.size() > 0)
	{
		_currentPath = nextPaths[0].path;
	}
}


void AIAgent::move(const glm::vec3& point1, const glm::vec3& point2)
{
	float distance = glm::distance(point1, point2);
	glm::vec3 dir = glm::normalize(point2 - point1);

	if (_t < distance)
	{
		glm::vec3 position = point1 + _t * dir;

		getSceneObject()->setPosition(position);
	}
	else
	{
		_t = _t - distance;
		++_currentPointIndex;

		if (_currentPath->getCurvePoints().size() > _currentPointIndex + 1)
		{
			const auto& point1 = _currentPath->getCurvePoints()[_currentPointIndex];
			const auto& point2 = _currentPath->getCurvePoints()[_currentPointIndex + 1];

			move(point1, point2);
		}
		else
		{
			reset();
		}
	}

	/*{
		//glm::vec3 right = glm::cross(dir, glm::vec3(0.0f, 1.0f, 0.0f));
		//glm::vec3 up = glm::cross(right, dir);

		dir.y = 0.0f;

		glm::vec3 rotationAxis = glm::cross(dir, glm::vec3(0.0f, 0.0f, 1.0f));
		//float rotationAngle = acos(glm::dot(dir, glm::vec3(0.0f, 0.0f, 1.0f)));
		float rotationAngle = glm::orientedAngle(glm::normalize(dir), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		LOG_INFO(LOG_VARIABLE(rotationAngle));

		rotationAxis = glm::normalize(rotationAxis);
		rotationAxis = glm::vec3(0.0f, 1.0f, 0.0f);

		glm::quat q;
		q.x = rotationAxis.x * sin(rotationAngle / 2);
		q.y = rotationAxis.y * sin(rotationAngle / 2);
		q.z = rotationAxis.z * sin(rotationAngle / 2);
		q.w = cos(rotationAngle / 2);
		getSceneObject()->setRotationQuaternion(q);

		//getSceneObject()->setRotationQuaternion(glm::quat(angle, rotationAxis.x, rotationAxis.y, rotationAxis.z));


		//glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f);
		//float rotationAngle = _t;
	}*/
}


void AIAgent::setSpeed(float speed)
{
	_speed = speed;

	if (_skeletalAnimationComponent != nullptr)
	{
		float ratio = _speed / _speedInAnimation;

		_skeletalAnimationComponent->setAnimationTicksPerSecond(_baseAnimationTicksPerSecond * ratio);
	}
}


float AIAgent::getSpeed()
{
	return _speed;
}


void AIAgent::setCurrentPath(PathComponent* path)
{
	_currentPath = path;
}


void AIAgent::update(float deltaTime)
{
	if (_currentPath != nullptr && _currentPath->getCurvePoints().size() > _currentPointIndex + 1)
	{
		const auto& point1 = _currentPath->getCurvePoints()[_currentPointIndex];
		const auto& point2 = _currentPath->getCurvePoints()[_currentPointIndex + 1];

		_t += deltaTime * _speed;

		move(point1, point2);
	}

	if (_currentPath != nullptr)
	{
		const auto& point1 = _currentPath->getCurvePoints()[_currentPointIndex];
		const auto& point2 = _currentPath->getCurvePoints()[_currentPointIndex + 1];

		glm::vec3 dir1 = glm::normalize(point2 - point1);
		glm::vec3 dir2 = _currentPointIndex + 2 < _currentPath->getCurvePoints().size() ? glm::normalize(_currentPath->getCurvePoints()[_currentPointIndex + 2] - point2) : dir1;

		glm::quat startRotation = QuaternionUtils::rotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), dir1);
		glm::quat destRotation = QuaternionUtils::rotationBetweenVectors(glm::vec3(0.0f, 0.0f, 1.0f), dir2);

		float distance = glm::distance(point1, point2);

		//glm::quat q = glm::mix(startRotation, destRotation, _t / distance);
		glm::quat q = glm::slerp(startRotation, destRotation, _t / distance);
		getSceneObject()->setRotationQuaternion(q);
	}
}
