#include "TransformAnimatorComponent.h"

#include "TransformCompletedEvent.h"

#include "../Scene/SceneObject.h"


TransformAnimatorComponent::TransformAnimatorComponent()
	: Component(CT_TRANSFORM_ANIMATOR),
	_translationTime(0.0f), _rotationTime(0.0f), _rotationLocalTime(0.0f), _scaleTime(0.0f),
	_translationSpeed(0.0f, 0.0f, 0.0f), _rotationSpeed(0.0f, 0.0f, 0.0f), _rotationLocalSpeed(0.0f, 0.0f, 0.0f), _scaleSpeed(0.0f, 0.0f, 0.0f)
{

}

void TransformAnimatorComponent::move(const glm::vec3& deltaPosition, float time)
{
	_translationSpeed = deltaPosition / time;
	_translationTime = time;
}


void TransformAnimatorComponent::move(float dx, float dy, float dz, float time)
{
	move(glm::vec3(dx, dy, dz), time);
}


void TransformAnimatorComponent::rotate(const glm::vec3& deltaRotation, float time)
{
	_rotationSpeed = deltaRotation / time;
	_rotationTime = time;
}


void TransformAnimatorComponent::rotate(float dx, float dy, float dz, float time)
{
	rotate(glm::vec3(dx, dy, dz), time);
}


void TransformAnimatorComponent::rotateLocal(const glm::vec3& deltaRotation, float time)
{
	_rotationLocalSpeed = deltaRotation / time;
	_rotationLocalTime = time;
}


void TransformAnimatorComponent::rotateLocal(float dx, float dy, float dz, float time)
{
	rotateLocal(glm::vec3(dx, dy, dz), time);
}


void TransformAnimatorComponent::scale(const glm::vec3& scale, float time)
{
	glm::vec3 targetScale = getSceneObject()->getScale() * scale;
	_scaleSpeed = (targetScale - getSceneObject()->getScale()) / time;
	_scaleTime = time;
}


void TransformAnimatorComponent::scale(float dx, float dy, float dz, float time)
{
	scale(glm::vec3(dx, dy, dz), time);
}


void TransformAnimatorComponent::update(float deltaTime)
{
	if (_translationTime > 0.0f)
	{
		if (_translationTime - deltaTime > 0.0f)
		{
			getSceneObject()->move(_translationSpeed * deltaTime);

			_translationTime -= deltaTime;
		}
		else
		{
			getSceneObject()->move(_translationSpeed * _translationTime);

			_translationTime -= deltaTime;

			getSceneObject()->dispatchEvent<TransformCompletedEvent>({ TransformCompletedEventType::TRANSLATION });
		}
	}

	if (_rotationTime > 0.0f)
	{
		if (_rotationTime - deltaTime > 0.0f)
		{
			getSceneObject()->rotate(_rotationSpeed * deltaTime);

			_rotationTime -= deltaTime;
		}
		else
		{
			getSceneObject()->rotate(_rotationSpeed * _rotationTime);

			_rotationTime -= deltaTime;

			getSceneObject()->dispatchEvent<TransformCompletedEvent>({ TransformCompletedEventType::ROTATION });
		}
	}

	if (_rotationLocalTime > 0.0f)
	{
		if (_rotationLocalTime - deltaTime > 0.0f)
		{
			getSceneObject()->rotateLocal(_rotationLocalSpeed * deltaTime);

			_rotationLocalTime -= deltaTime;
		}
		else
		{
			getSceneObject()->rotateLocal(_rotationLocalSpeed * _rotationLocalTime);

			_rotationLocalTime -= deltaTime;

			getSceneObject()->dispatchEvent<TransformCompletedEvent>({ TransformCompletedEventType::ROTATION_LOCAL });
		}
	}

	if (_scaleTime > 0.0f)
	{
		if (_scaleTime - deltaTime > 0.0f)
		{
			getSceneObject()->setScale(getSceneObject()->getScale() + _scaleSpeed * deltaTime);

			_scaleTime -= deltaTime;
		}
		else
		{
			getSceneObject()->setScale(getSceneObject()->getScale() + _scaleSpeed * _scaleTime);

			_scaleTime -= deltaTime;

			getSceneObject()->dispatchEvent<TransformCompletedEvent>({ TransformCompletedEventType::SCALE });
		}
	}
}
