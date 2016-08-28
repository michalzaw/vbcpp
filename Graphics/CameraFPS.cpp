#include "CameraFPS.hpp"


CameraFPS::CameraFPS(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
    : CameraStatic(width, height, viewAngle, nearValue, farValue),
	m_MoveSpeed(3.0f), m_RotateSpeed(0.0005f)//, _oldXpos(0), _oldYpos(0)
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
	float horizontalAngle = _objectTransform->GetRotation().y + m_RotateSpeed * GLdouble( _windowWidth/2 - mousePosX);
	float verticalAngle = _objectTransform->GetRotation().x + m_RotateSpeed * GLdouble( _windowHeight/2 - mousePosY);
	//m_HorizontalAngle += m_RotateSpeed * GLdouble( m_WindowWidth/2 - mousePosX);
	//m_VerticalAngle += m_RotateSpeed * GLdouble( m_WindowHeight/2 - mousePosY);

    //m_HorizontalAngle += m_RotateSpeed * GLdouble( _oldXpos - mousePosX);
	//m_VerticalAngle += m_RotateSpeed * GLdouble( _oldYpos - mousePosY);


	if (horizontalAngle > 2 * PI || horizontalAngle < -2 * PI) horizontalAngle = 0.0f;

	if (verticalAngle > 0.4f * PI) verticalAngle = 0.4f * PI;
	if (verticalAngle < -0.4f * PI) verticalAngle = -0.4f * PI;

	_objectTransform->SetRotation(glm::vec3(verticalAngle, horizontalAngle, 0.0f));

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
	//m_Direction *= deltaTime * m_MoveSpeed;

	//m_Position += m_Direction;

	//update();

	glm::vec3 dir = getDirection();

	dir *= deltaTime * m_MoveSpeed;

	_objectTransform->SetPosition(_objectTransform->GetPosition() + dir);
}


void CameraFPS::moveBackward(double deltaTime)
{
	//m_Direction *= deltaTime * m_MoveSpeed;

	//m_Position -= m_Direction;

	//update();

	glm::vec3 dir = getDirection();

	dir *= deltaTime * m_MoveSpeed;

	_objectTransform->SetPosition(_objectTransform->GetPosition() - dir);
}


void CameraFPS::strafeLeft(double deltaTime)
{
	//m_RightVector *= deltaTime * m_MoveSpeed;

	//m_Position -= m_RightVector;

	//update();

	glm::vec3 dir = getRightVector();

	dir *= deltaTime * m_MoveSpeed;

	_objectTransform->SetPosition(_objectTransform->GetPosition() - dir);
}


void CameraFPS::strafeRight(double deltaTime)
{
	//m_RightVector *= deltaTime * m_MoveSpeed;

	//m_Position += m_RightVector;

	//update();

	glm::vec3 dir = getRightVector();

	dir *= deltaTime * m_MoveSpeed;

	_objectTransform->SetPosition(_objectTransform->GetPosition() + dir);
}


void CameraFPS::setMoveSpeed(float speed)
{
	m_MoveSpeed = speed;
}


void CameraFPS::setRotationSpeed(float speed)
{
	m_RotateSpeed = speed;
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
