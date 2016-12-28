#include "CameraFPS.hpp"

#include "../Scene/SceneObject.h"


CameraFPS::CameraFPS(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
    : CameraStatic(width, height, viewAngle, nearValue, farValue),
	_moveSpeed(3.0f), _rotateSpeed(0.0005f)//, _oldXpos(0), _oldYpos(0)
{
    //update();

	//updateProjection();
	//updateView();
}


CameraFPS::~CameraFPS()
{

}


/*void CameraFPS::update()
{
	updateDirection();

	m_RightVector = vec3(
				sin(m_HorizontalAngle - 3.14f/2.0f),
				0,
				cos(m_HorizontalAngle - 3.14f/2.0f)
	);

	m_UpVector = vec3( glm::cross(m_RightVector, m_Direction) );

	m_LookAt = m_Position + m_Direction;

	m_ViewMatrix = glm::lookAt(
						m_Position,
						m_LookAt,
						m_UpVector );
}*/


void CameraFPS::setRotation(int mousePosX, int mousePosY)
{
	// Compute new orientation
	float horizontalAngle = _object->getRotation().y + _rotateSpeed * GLdouble(_windowWidth / 2 - mousePosX);
	float verticalAngle = _object->getRotation().x + _rotateSpeed * GLdouble( _windowHeight/2 - mousePosY);
	//float horizontalAngle = _objectTransform->getRotation().y + _rotateSpeed * GLdouble( _windowWidth/2 - mousePosX);
	//float verticalAngle = _objectTransform->getRotation().x + _rotateSpeed * GLdouble( _windowHeight/2 - mousePosY);
	//m_HorizontalAngle += _rotateSpeed * GLdouble( m_WindowWidth/2 - mousePosX);
	//m_VerticalAngle += _rotateSpeed * GLdouble( m_WindowHeight/2 - mousePosY);

    //m_HorizontalAngle += _rotateSpeed * GLdouble( _oldXpos - mousePosX);
	//m_VerticalAngle += _rotateSpeed * GLdouble( _oldYpos - mousePosY);


	if (horizontalAngle > 2 * PI || horizontalAngle < -2 * PI) horizontalAngle = 0.0f;

	if (verticalAngle > 0.4f * PI) verticalAngle = 0.4f * PI;
	if (verticalAngle < -0.4f * PI) verticalAngle = -0.4f * PI;

	//_objectTransform->setRotation(glm::vec3(verticalAngle, horizontalAngle, 0.0f));
	_object->setRotation(verticalAngle, horizontalAngle, 0.0f);

	//update();
}


/*void CameraFPS::updateDirection()
{
	// Direction: coordinates convertion from Spherical to Cartesian
	m_Direction = vec3(
				cos(m_VerticalAngle) * sin(m_HorizontalAngle),
				sin(m_VerticalAngle),
				cos(m_VerticalAngle) * cos(m_HorizontalAngle)
	);
}*/


void CameraFPS::moveForward(double deltaTime)
{
	//m_Direction *= deltaTime * _moveSpeed;

	//m_Position += m_Direction;

	//update();

	glm::vec3 dir = getLocalDirection();

	dir *= deltaTime * _moveSpeed;

	//_objectTransform->setPosition(_objectTransform->getPosition() + dir);
	_object->move(dir);
}


void CameraFPS::moveBackward(double deltaTime)
{
	//m_Direction *= deltaTime * _moveSpeed;

	//m_Position -= m_Direction;

	//update();

	glm::vec3 dir = getLocalDirection();

	dir *= deltaTime * _moveSpeed;

	//_objectTransform->setPosition(_objectTransform->getPosition() - dir);
	_object->move(-dir);
}


void CameraFPS::strafeLeft(double deltaTime)
{
	//m_RightVector *= deltaTime * _moveSpeed;

	//m_Position -= m_RightVector;

	//update();

	glm::vec3 dir = getLocalRightVector();

	dir *= deltaTime * _moveSpeed;

	//_objectTransform->setPosition(_objectTransform->getPosition() - dir);
	_object->move(-dir);
}


void CameraFPS::strafeRight(double deltaTime)
{
	//m_RightVector *= deltaTime * _moveSpeed;

	//m_Position += m_RightVector;

	//update();

	glm::vec3 dir = getLocalRightVector();

	dir *= deltaTime * _moveSpeed;

	//_objectTransform->setPosition(_objectTransform->getPosition() + dir);
	_object->move(dir);
}


void CameraFPS::setMoveSpeed(float speed)
{
	_moveSpeed = speed;
}


void CameraFPS::setRotationSpeed(float speed)
{
	_rotateSpeed = speed;
}

/*void CameraFPS::setCursorPos(GLdouble xpos, GLdouble ypos)
{
    _oldXpos = xpos;
    _oldYpos = ypos;

    update();
}*/

void CameraFPS::reset()
{
    //m_HorizontalAngle = 3.14f;
    //m_VerticalAngle = 0.0f;
    //m_Direction = glm::vec3(0,0,0);

    //update();
}
