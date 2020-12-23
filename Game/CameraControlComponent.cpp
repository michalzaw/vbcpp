#include "CameraControlComponent.h"

#include <glm/gtx/transform.hpp>

#include "../Scene/SceneManager.h"
#include "../Scene/SceneObject.h"

#include "../Graphics/CameraFPS.hpp"

#include "../Utils/Helpers.hpp"
#include "../Utils/InputSystem.h"


CameraControlComponent::CameraControlComponent(CameraFPS* camera)
	: Component(CT_CAMERA_CONTROL),
	_camera(camera),
	_movmentControl(true), _rotationControl(true),
	_changePositionOffset(false), _newPositionOffset(0.0f)
{
	_isActive = false;
}


void CameraControlComponent::setMovmentControl(bool isEnable)
{
	_movmentControl = isEnable;
}


void CameraControlComponent::setRotationControl(bool isEnable)
{
	_rotationControl = isEnable;
}


bool CameraControlComponent::isMOvementControl()
{
	return _movmentControl;
}


bool CameraControlComponent::isRotationControl()
{
	return _rotationControl;
}


void CameraControlComponent::update(float deltaTime)
{
	if (!_isActive)
	{
		return;
	}

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
		//if (_isCameraControll)
		{
			double xpos, ypos;
			inputSystem.getCursorPosition(&xpos, &ypos);
			inputSystem.setCursorPosition(_camera->getWindowWidth() / 2, _camera->getWindowHeight() / 2);

			_camera->setRotation(xpos, ypos);
		}
	}

	float minPositionOffset = _camera->getMinPositionOffset();
	float maxPositionOffset = _camera->getMaxPositionOffset();

	if (minPositionOffset != 0.0f && maxPositionOffset != 0.0f)
	{
		double scrollOffsetY = inputSystem.getScrollOffsetY();

		_newPositionOffset = _camera->getPositionOffset() + scrollOffsetY;
		_newPositionOffset = clamp(_newPositionOffset, minPositionOffset, maxPositionOffset);

		_camera->setPositionOffset(_newPositionOffset);
	}




	if (_camera->getSceneObject()->getParent() == nullptr && _camera->getMinPositionOffset() == 0.0f && _camera->getMaxPositionOffset() == 0.0f)
	{
		const glm::vec3 rayOrigin = _camera->getPosition() - glm::vec3(0.0f, 1.5f, 0.0f);
		const glm::vec3 rayDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		const short int filterMask = COL_TERRAIN | COL_ENV;
		const short int filterGroup = COL_WHEEL;

		glm::vec3 hitPosition;
		if (_camera->getSceneObject()->getSceneManager()->getPhysicsManager()->rayTest(rayOrigin, rayDirection, filterMask, filterGroup, hitPosition))
		{
			const glm::vec3 cameraPosition = _camera->getPosition();
			_camera->getSceneObject()->setPosition(cameraPosition.x, hitPosition.y + 1.5f, cameraPosition.z);
		}
	}
	else if (_camera->getSceneObject()->getParent() != nullptr && (_camera->getMinPositionOffset() != 0.0f || _camera->getMaxPositionOffset() != 0.0f))
	{
		const glm::vec3 cameraRotation = _camera->getSceneObject()->getRotation();
		if (cameraRotation.x >= 0.0f)
		{
			_camera->getSceneObject()->setRotation(0.0f, cameraRotation.y, cameraRotation.z);
		}
	}
}