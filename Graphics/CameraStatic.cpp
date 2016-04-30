#include "CameraStatic.hpp"


CameraStatic::CameraStatic(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
	: m_ProjectionMatrix(1.0), m_ViewMatrix(1.0),
	m_Position(vec3(0,0,-10)), m_LookAt(vec3(0,0,0)), m_UpVector(vec3(0,1,0)),
    m_FarValue(farValue), m_NearValue(nearValue), m_ViewAngle(viewAngle), m_WindowWidth(width), m_WindowHeight(height)
{
	updateProjection();
	updateView();
}


CameraStatic::~CameraStatic()
{

}


vec3 CameraStatic::getPosition()
{
	return m_Position;
}


GLfloat CameraStatic::getNearValue()
{
	return m_NearValue;
}


GLfloat	CameraStatic::getFarValue()
{
	return m_FarValue;
}


glm::vec3 CameraStatic::getLookAtVector()
{
	return m_LookAt;
}

glm::vec3 CameraStatic::getUpVector()
{
	return m_UpVector;
}


glm::mat4 CameraStatic::getProjectionMatrix()
{
	return m_ProjectionMatrix;
}


glm::mat4 CameraStatic::getViewMatrix()
{
	return m_ViewMatrix;
}


void CameraStatic::setPosition(vec3 position)
{
	m_Position = position;

	updateView();
}


void CameraStatic::setPosition(GLfloat x, GLfloat y, GLfloat z)
{
	vec3 pos(x, y, z);

	setPosition(pos);
}


void CameraStatic::lookAt(vec3 lookAt)
{
	m_LookAt = lookAt;

	updateView();
}


void CameraStatic::setUpVector(vec3 upVector)
{
	m_UpVector = upVector;

	updateView();
}


void CameraStatic::setFarValue(GLfloat value)
{
	m_FarValue = value;

	updateProjection();
}


void CameraStatic::setNearValue(GLfloat value)
{
	m_NearValue = value;

	updateProjection();
}


void CameraStatic::setViewAngle(GLfloat angle)
{
	m_ViewAngle = angle;

	updateProjection();
}


void CameraStatic::setWindowDimensions(GLint width, GLint height)
{
	m_WindowWidth = width;
	m_WindowHeight = height;

	updateProjection();
}


void CameraStatic::updateProjection()
{
	m_ProjectionMatrix = glm::perspective(m_ViewAngle, float(m_WindowWidth) / float(m_WindowHeight), m_NearValue, m_FarValue);
}


void CameraStatic::updateView()
{
	m_ViewMatrix = glm::lookAt(
						m_Position,
						m_LookAt,
						m_UpVector );
}
