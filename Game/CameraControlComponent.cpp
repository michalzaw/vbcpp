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

	float minPositionOffset = _camera->getMinPositionOffset();
	float maxPositionOffset = _camera->getMaxPositionOffset();

	if (minPositionOffset != 0.0f && maxPositionOffset != 0.0f)
	{
		double scrollOffsetY = inputSystem.getScrollOffsetY();

		float newOffset = _camera->getPositionOffset() + scrollOffsetY;
		newOffset = clamp(newOffset, minPositionOffset, maxPositionOffset);

		_camera->setPositionOffset(newOffset);
	}



	SceneObject* terrainSceneObject = _camera->getSceneObject()->getSceneManager()->getSceneObject("terrain");

	glm::vec3 rayOrigin = _camera->getPosition() - glm::vec3(0.0f, 1.5f, 0.0f);
	glm::vec3 rayEnd = rayOrigin + glm::vec3(0.0f, 1.0f, 0.0f) * 1000.0f;

	btCollisionWorld::ClosestRayResultCallback RayCallback(
		btVector3(rayOrigin.x, rayOrigin.y, rayOrigin.z),
		btVector3(rayEnd.x, rayEnd.y, rayEnd.z)
	);

	RayCallback.m_collisionFilterMask = COL_TERRAIN | COL_ENV;
	RayCallback.m_collisionFilterGroup = COL_WHEEL;

	terrainSceneObject->getSceneManager()->getPhysicsManager()->getDynamicsWorld()->rayTest(
		btVector3(rayOrigin.x, rayOrigin.y, rayOrigin.z),
		btVector3(rayEnd.x, rayEnd.y, rayEnd.z),
		RayCallback
	);

	if (RayCallback.hasHit()) {

		if (_camera->getMinPositionOffset() == 0.0f && _camera->getMaxPositionOffset() == 0.0f)
		{
			glm::vec3 cameraPosition = _camera->getPosition();
			_camera->getSceneObject()->setPosition(cameraPosition.x, RayCallback.m_hitPointWorld.y() + 1.5, cameraPosition.z);
		}
		else
		{
			/*glm::vec3 direction = glm::vec3(RayCallback.m_hitPointWorld.x(), RayCallback.m_hitPointWorld.y(), RayCallback.m_hitPointWorld.z()) - _camera->getPosition();

			glm::mat4 inverseModelMatrix = glm::inverse(_camera->getSceneObject()->getParent()->getGlobalTransformMatrix());
			glm::vec4 d = inverseModelMatrix * glm::vec4(direction.x, direction.y, direction.z, 0.0f);

			_camera->getSceneObject()->setPosition(_camera->getSceneObject()->getPosition() + glm::vec3(d.x, d.y, d.z));*/

			float distanceToTerrain = glm::distance(glm::vec3(RayCallback.m_hitPointWorld.x(), RayCallback.m_hitPointWorld.y(), RayCallback.m_hitPointWorld.z()), _camera->getPosition());

			_camera->setPositionOffset(_camera->getPositionOffset() - distanceToTerrain + 1.5f);

		}
	}
	else {
	}




	/*SceneObject* terrainSceneObject = _camera->getSceneObject()->getSceneManager()->getSceneObject("terrain");
	Terrain* terrainComponent = static_cast<Terrain*>(terrainSceneObject->getComponent(CT_TERRAIN));

	float height = terrainComponent->getHeight(_camera->getPosition().x, _camera->getPosition().y);

	std::cout << height << std::endl;

	if (height > _camera->getPosition().y)
	{

	}
	else
	{

	}*/
}