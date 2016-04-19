#include "CameraFPS.hpp"


CameraFPS::CameraFPS(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
: CameraStatic(width, height, viewAngle, nearValue, farValue),
	m_HorizontalAngle(0.0f), m_VerticalAngle(0.0f),
	m_MoveSpeed(3.0f), m_RotateSpeed(0.0005f), m_Direction(vec3(0,0,0)), m_RightVector( vec3(0,0,0)), _oldXpos(0), _oldYpos(0)
{
    update();

	updateProjection();
	updateView();
}


CameraFPS::~CameraFPS()
{

}


void CameraFPS::update()
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
}


void CameraFPS::setRotation(int mousePosX, int mousePosY)
{
	// Compute new orientation

	GLdouble newXpos = m_WindowWidth / 2.0f - mousePosX; //_oldXpos - mousePosX;
	GLdouble newYpos = m_WindowHeight / 2.0f - mousePosY; //_oldYpos - mousePosY;

	//m_HorizontalAngle += m_RotateSpeed * GLdouble( m_WindowWidth/2 - mousePosX);
	//m_VerticalAngle += m_RotateSpeed * GLdouble( m_WindowHeight/2 - mousePosY);

	m_HorizontalAngle += m_RotateSpeed * newXpos;
	m_VerticalAngle += m_RotateSpeed * newYpos;

	if (m_HorizontalAngle > 2 * PI || m_HorizontalAngle < -2 * PI) m_HorizontalAngle = 0.0f;

	if (m_VerticalAngle > 0.4f * PI) m_VerticalAngle = 0.4f * PI;
	if (m_VerticalAngle < -0.4f * PI) m_VerticalAngle = -0.4f * PI;

	update();
}


void CameraFPS::updateDirection()
{
	// Direction: coordinates convertion from Spherical to Cartesian
	m_Direction = vec3(
				cos(m_VerticalAngle) * sin(m_HorizontalAngle),
				sin(m_VerticalAngle),
				cos(m_VerticalAngle) * cos(m_HorizontalAngle)
	);
}


void CameraFPS::moveForward(double deltaTime)
{
	m_Direction *= deltaTime * m_MoveSpeed;

	m_Position += m_Direction;

	update();
}


void CameraFPS::moveBackward(double deltaTime)
{
	m_Direction *= deltaTime * m_MoveSpeed;

	m_Position -= m_Direction;

	update();
}


void CameraFPS::strafeLeft(double deltaTime)
{
	m_RightVector *= deltaTime * m_MoveSpeed;

	m_Position -= m_RightVector;

	update();
}


void CameraFPS::strafeRight(double deltaTime)
{
	m_RightVector *= deltaTime * m_MoveSpeed;

	m_Position += m_RightVector;

	update();
}


void CameraFPS::setMoveSpeed(float speed)
{
	m_MoveSpeed = speed;
}


void CameraFPS::setRotationSpeed(float speed)
{
	m_RotateSpeed = speed;
}

void CameraFPS::setCursorPos(GLdouble xpos, GLdouble ypos)
{
    _oldXpos = xpos;
    _oldYpos = ypos;
}

void CameraFPS::reset()
{
    m_HorizontalAngle = 0.0f;
    m_VerticalAngle = 0.0f;
    m_Direction = glm::vec3(0,0,0);

    update();
}
