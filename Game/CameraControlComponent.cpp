#include "CameraControlComponent.h"

#include "../Graphics/CameraFPS.hpp"

#include "../Utils/InputSystem.h"


CameraControlComponent::CameraControlComponent(CameraFPS* camera)
	: Component(CT_CAMERA_CONTROL),
	_camera(camera),
	_movmentControl(true), _rotationControl(true)
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
}