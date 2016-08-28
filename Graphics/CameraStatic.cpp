#include "CameraStatic.hpp"


CameraStatic::CameraStatic(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
	: Component(CT_CAMERA),
	_projectionMatrix(1.0), _viewMatrix(1.0),
	_lookAt(vec3(0,0,0)), _upVector(vec3(0,1,0)),
    _farValue(farValue), _nearValue(nearValue), _viewAngle(viewAngle), _windowWidth(width), _windowHeight(height)
{
    changedTransform();

    _projectionMatrixIs = false;
	//updateProjection();
	//updateView();
}


CameraStatic::~CameraStatic()
{

}


vec3 CameraStatic::getPosition()
{
	return _objectTransform->GetPosition();
}


GLfloat CameraStatic::getNearValue()
{
	return _nearValue;
}


GLfloat	CameraStatic::getFarValue()
{
	return _farValue;
}


GLfloat	CameraStatic::getViewAngle()
{
	return _viewAngle;
}


glm::vec3 CameraStatic::getLookAtVector()
{
    if (!_lookAtIs)
    {
        _lookAt = getPosition() + getDirection();

        _lookAtIs = true;
    }

	return _lookAt;
}

glm::vec3 CameraStatic::getUpVector()
{
    if (!_upVectorIs)
    {
        _upVector = glm::cross(getRightVector(), getDirection());

        _upVectorIs = true;
    }

	return _upVector;
}

glm::vec3 CameraStatic::getDirection()
{
    if (!_directionIs)
    {
        float verticalAngle = _objectTransform->GetRotation().x;
        float horizontalAngle = _objectTransform->GetRotation().y;

        _direction = vec3(
                    cos(verticalAngle) * sin(horizontalAngle),
                    sin(verticalAngle),
                    cos(verticalAngle) * cos(horizontalAngle)
        );

        _directionIs = true;
    }

	return _direction;
}

glm::vec3 CameraStatic::getRightVector()
{
    if (!_rightVectorIs)
    {
        float horizontalAngle = _objectTransform->GetRotation().y;

        _rightVector = vec3(
				sin(horizontalAngle - PI/2.0f),
				0,
				cos(horizontalAngle - PI/2.0f)
        );

        _rightVectorIs = true;
    }

    return _rightVector;
}


glm::mat4 CameraStatic::getProjectionMatrix()
{
    if (!_projectionMatrixIs)
    {
        _projectionMatrix = glm::perspective(_viewAngle, float(_windowWidth) / float(_windowHeight), _nearValue, _farValue);

        _projectionMatrixIs = true;
    }

	return _projectionMatrix;
}


glm::mat4 CameraStatic::getViewMatrix()
{
    if (!_viewMatrixIs)
    {
        _viewMatrix = glm::lookAt(
						getPosition(),
						getLookAtVector(),
						getUpVector() );

        _viewMatrixIs = true;
    }

	return _viewMatrix;
}


void CameraStatic::setFarValue(GLfloat value)
{
	_farValue = value;

	_projectionMatrixIs = false;//updateProjection();
}


void CameraStatic::setNearValue(GLfloat value)
{
	_nearValue = value;

	_projectionMatrixIs = false;//updateProjection();
}


void CameraStatic::setViewAngle(GLfloat angle)
{
	_viewAngle = angle;

	_projectionMatrixIs = false;//updateProjection();
}


void CameraStatic::setWindowDimensions(GLint width, GLint height)
{
	_windowWidth = width;
	_windowHeight = height;

	_projectionMatrixIs = false;//updateProjection();
}


/*void CameraStatic::updateProjection()
{
	m_ProjectionMatrix = glm::perspective(m_ViewAngle, float(m_WindowWidth) / float(m_WindowHeight), m_NearValue, m_FarValue);
}


void CameraStatic::updateView()
{
	m_ViewMatrix = glm::lookAt(
						m_Position,
						m_LookAt,
						m_UpVector );
}*/
