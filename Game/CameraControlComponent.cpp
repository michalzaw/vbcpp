#include "CameraControlComponent.h"

#include <glm/gtx/transform.hpp>

#include "../Scene/SceneManager.h"
#include "../Scene/SceneObject.h"

#include "../Graphics/CameraFPS.hpp"

#include "../Utils/InputSystem.h"


CameraControlComponent::CameraControlComponent(CameraFPS* camera)
	: Component(CT_CAMERA_CONTROL),
	_camera(camera),
	_movmentControl(true), _rotationControl(true),
	_changePositionOffsetSpeed(10.0f), _deltaPositionOffset(0.0f),
	RAY_TEST_FILTER_MASK(COL_TERRAIN | COL_ENV), RAY_TEST_FILTER_GROUP(COL_WHEEL)
{
	_isActive = false;
}


void CameraControlComponent::updateFromInput(float deltaTime)
{
	InputSystem& inputSystem = InputSystem::getInstance();

	if (_movmentControl)
	{
		if (inputSystem.isKeyDown(GLFW_KEY_W))
		{
			_camera->moveForward(deltaTime);
		}
		if (inputSystem.isKeyDown(GLFW_KEY_S))
		{
			_camera->moveBackward(deltaTime);
		}
		if (inputSystem.isKeyDown(GLFW_KEY_D))
		{
			_camera->strafeRight(deltaTime);
		}
		if (inputSystem.isKeyDown(GLFW_KEY_A))
		{
			_camera->strafeLeft(deltaTime);
		}
	}

	if (_rotationControl)
	{
		double xpos, ypos;
		inputSystem.getCursorPosition(&xpos, &ypos);
		inputSystem.setCursorPosition(_camera->getWindowWidth() / 2, _camera->getWindowHeight() / 2);

		_camera->setRotation(xpos, ypos);
	}

	if (_camera->getMinPositionOffset() != 0.0f && _camera->getMaxPositionOffset() != 0.0f)
	{
		_deltaPositionOffset += inputSystem.getScrollOffsetY();
	}
}


void CameraControlComponent::updateCameraPositionOffset(float deltaTime)
{
	float delta = 0.0f;

	if (_deltaPositionOffset < 0.0f)
	{
		delta = -_changePositionOffsetSpeed * deltaTime;

		if (delta < _deltaPositionOffset)
		{
			delta = _deltaPositionOffset;
		}
	}
	else if (_deltaPositionOffset > 0.0f)
	{
		delta = _changePositionOffsetSpeed * deltaTime;

		if (delta > _deltaPositionOffset)
		{
			delta = _deltaPositionOffset;
		}
	}

	_deltaPositionOffset -= delta;
	_camera->setPositionOffset(_camera->getPositionOffset() + delta);
}


void CameraControlComponent::moveCameraAboveTerrain()
{
	const glm::vec3 rayOrigin = _camera->getPosition() - glm::vec3(0.0f, 1.5f, 0.0f);
	const glm::vec3 rayDirection = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 hitPosition;
	if (_camera->getSceneObject()->getSceneManager()->getPhysicsManager()->rayTest(rayOrigin, rayDirection, RAY_TEST_FILTER_MASK, RAY_TEST_FILTER_GROUP, hitPosition))
	{
		const glm::vec3 cameraPosition = _camera->getPosition();
		_camera->getSceneObject()->setPosition(cameraPosition.x, hitPosition.y + 1.5f, cameraPosition.z);
	}
}


void CameraControlComponent::setMovmentControl(bool isEnable)
{
	_movmentControl = isEnable;
}


void CameraControlComponent::setRotationControl(bool isEnable)
{
	_rotationControl = isEnable;
}


void CameraControlComponent::setChangePositionOffsetSpeed(float speed)
{
	_changePositionOffsetSpeed = speed;
}


bool CameraControlComponent::isMOvementControl()
{
	return _movmentControl;
}


bool CameraControlComponent::isRotationControl()
{
	return _rotationControl;
}


float CameraControlComponent::getChangePositionOffsetSpeed()
{
	return _changePositionOffsetSpeed;
}


void CameraControlComponent::update(float deltaTime)
{
	if (_isActive)
	{
		updateFromInput(deltaTime);
	}

	const float minPositionOffset = _camera->getMinPositionOffset();
	const float maxPositionOffset = _camera->getMaxPositionOffset();

	if (minPositionOffset != 0.0f || maxPositionOffset != 0.0f)
	{
		updateCameraPositionOffset(deltaTime);
	}

	if (_camera->getSceneObject()->getParent() == nullptr && minPositionOffset == 0.0f && maxPositionOffset == 0.0f)
	{
		moveCameraAboveTerrain();
	}
}