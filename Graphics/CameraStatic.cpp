#include "CameraStatic.hpp"

#include "../Scene/SceneObject.h"


/*CameraStatic::CameraStatic(int width, int height, GLfloat viewAngle, GLfloat nearValue, GLfloat farValue)
	: Component(CT_CAMERA),
	_projectionMatrix(1.0), _viewMatrix(1.0),
	_lookAt(vec3(0,0,0)), _upVector(vec3(0,1,0)),
    _farValue(farValue), _nearValue(nearValue), _viewAngle(viewAngle), _windowWidth(width), _windowHeight(height)
{
    changedTransform();

    _projectionMatrixIs = false;
	//updateProjection();
	//updateView();
}*/


CameraStatic::CameraStatic(CameraProjectionType projectionType)
	: Component(CT_CAMERA),
	_projectionType(projectionType),
	_projectionMatrix(1.0), _viewMatrix(1.0),
	_lookAt(vec3(0,0,0)), _upVector(vec3(0,1,0)),
    _farValue(0.1f), _nearValue(1000.0f), _viewAngle(45.0f), _windowWidth(800), _windowHeight(600),
    _left(-1.0f), _right(1.0f), _bottom(-1.0f), _top(1.0f)
{
    changedTransform();

    _projectionMatrixIs = false;
	//updateProjection();
	//updateView();
}


CameraStatic::~CameraStatic()
{

}


vec3 CameraStatic::transformToGlobal(glm::vec3 vector)
{
    glm::vec4 vec(vector, 0.0f);
    if (_object->hasParent())
        vec = _object->getParent()->getGlobalTransformMatrix() * vec;
        //vec = _object->getParent()->getGlobalTransform()->getNormalMatrix() * vec;

    return glm::normalize(glm::vec3(vec.x, vec.y, vec.z));
}


CameraProjectionType CameraStatic::getProjctionType()
{
    return _projectionType;
}


vec3 CameraStatic::getPosition()
{
    glm::vec4 pos = _object->getGlobalTransformMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    //glm::vec4 pos = getGlobalTransform()->getTransformMatrix() * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    return glm::vec3(pos.x, pos.y, pos.z);
    //return getGlobalTransform()->getPosition();
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


GLint CameraStatic::getWindowWidth()
{
    return _windowWidth;
}


GLint CameraStatic::getWindowHeight()
{
    return _windowHeight;
}


GLfloat CameraStatic::getLeft()
{
    return _left;
}


GLfloat CameraStatic::getRight()
{
    return _right;
}


GLfloat CameraStatic::getBottom()
{
    return _bottom;
}


GLfloat CameraStatic::getTop()
{
    return _top;
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
        _upVector = transformToGlobal(getLocalUpVector());

        _upVectorIs = true;
    }

	return _upVector;
}

glm::vec3 CameraStatic::getDirection()
{
    if (!_directionIs)
    {
        _direction = transformToGlobal(getLocalDirection());

        _directionIs = true;
    }

	return _direction;
}

glm::vec3 CameraStatic::getRightVector()
{
    if (!_rightVectorIs)
    {
        _rightVector = transformToGlobal(getLocalRightVector());

        _rightVectorIs = true;
    }

    return _rightVector;
}


vec3 CameraStatic::getLocalUpVector()
{
    if (!_localDirectionIs)
    {
        _localUpVector = glm::cross(getLocalRightVector(), getLocalDirection());

        _localUpVectorIs = true;
    }

	return _localUpVector;
}


vec3 CameraStatic::getLocalDirection()
{
    if (!_localDirectionIs)
    {
        //float verticalAngle = getGlobalTransform()->getRotation().x;
        //float horizontalAngle = getGlobalTransform()->getRotation().y;
        float verticalAngle = _object->getRotation().x;
        float horizontalAngle = _object->getRotation().y;


        _localDirection = glm::vec3(
                    cos(verticalAngle) * sin(horizontalAngle),
                    sin(verticalAngle),
                    cos(verticalAngle) * cos(horizontalAngle)
        );

        _localDirectionIs = true;
    }

	return _localDirection;
}


vec3 CameraStatic::getLocalRightVector()
{
    if (!_localRightVectorIs)
    {
        //float horizontalAngle = getGlobalTransform()->getRotation().y;
        float horizontalAngle = _object->getRotation().y;

        _localRightVector = glm::vec3(
				sin(horizontalAngle - PI/2.0f),
				0,
				cos(horizontalAngle - PI/2.0f)
        );

        _localRightVectorIs = true;
    }

	return _localRightVector;
}


glm::mat4 CameraStatic::getProjectionMatrix()
{
    if (!_projectionMatrixIs)
    {
        if (_projectionType == CPT_PERSPECTIVE)
            _projectionMatrix = glm::perspective(_viewAngle, float(_windowWidth) / float(_windowHeight), _nearValue, _farValue);

        else
            _projectionMatrix = glm::ortho(_left, _right, _bottom, _top, _nearValue, _farValue);

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


void CameraStatic::setLeft(GLfloat left)
{
    _left = left;

	_projectionMatrixIs = false;
}


void CameraStatic::setRight(GLfloat right)
{
    _right = right;

	_projectionMatrixIs = false;
}


void CameraStatic::setBottom(GLfloat bottom)
{
    _bottom = bottom;

	_projectionMatrixIs = false;
}


void CameraStatic::setTop(GLfloat top)
{
    _top = top;

	_projectionMatrixIs = false;
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
