#ifndef __CAMERASTATIC_HPP__
#define __CAMERASTATIC_HPP__

#include <cmath>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "AABB.h"
#include "Frustum.h"

#include "../Scene/Component.h"

#include "../Utils/Math.h"

using glm::vec3;
using glm::mat4;


enum CameraProjectionType
{
    CPT_PERSPECTIVE,
    CPT_ORTHOGRAPHIC
};


class CameraStatic : public Component
{
	public:
		//CameraStatic(int width = 800, int height = 600, GLfloat viewAngle = 45.0f, GLfloat nearValue = 0.1f, GLfloat farValue = 1000.0f);
        CameraStatic(CameraProjectionType projectionType);

		virtual ~CameraStatic();

		CameraProjectionType getProjctionType();

		vec3 getPosition();
		vec3 getLookAtVector();
		vec3 getUpVector();
		vec3 getDirection();
		vec3 getRightVector();
		vec3 getLocalUpVector();
		vec3 getLocalDirection();
		vec3 getLocalRightVector();

		GLfloat getNearValue();
		GLfloat	getFarValue();
		GLfloat getViewAngle();
		GLint   getWindowWidth();
		GLint   getWindowHeight();

		GLfloat getLeft();
		GLfloat getRight();
		GLfloat getBottom();
		GLfloat getTop();

		GLfloat getMinPositionOffset();
		GLfloat getMaxPositionOffset();
		GLfloat getPositionOffset();

		mat4 getProjectionMatrix();
		mat4 getViewMatrix();

		void setFarValue(GLfloat value);
		void setNearValue(GLfloat value);
		void setViewAngle(GLfloat angle);
		void setWindowDimensions(GLint width, GLint height);

		void setLeft(GLfloat left);
		void setRight(GLfloat right);
		void setBottom(GLfloat bottom);
		void setTop(GLfloat top);

		void setMinPositionOffset(GLfloat minValue);
		void setMaxPositionOffset(GLfloat maxValue);
		void setPositionOffset(GLfloat positionOffset);

		void setOrthoProjectionParams(GLfloat left, GLfloat right, GLfloat bottom, GLfloat top, GLfloat near, GLfloat far);

		AABB* getAABB();

		virtual void changedTransform()
		{
		    _viewMatrixIs = false;
		    _lookAtIs = false;
		    _upVectorIs = false;
		    _directionIs = false;
		    _rightVectorIs = false;
		    _localUpVectorIs = false;
		    _localDirectionIs = false;
		    _localRightVectorIs = false;
		    _aabbIs = false;
		}

	protected:
	    CameraProjectionType _projectionType;

		mat4	_projectionMatrix;         bool _projectionMatrixIs;
		mat4	_viewMatrix;               bool _viewMatrixIs;

		vec3	_lookAt;                   bool _lookAtIs;
		vec3	_upVector;                 bool _upVectorIs;
		vec3    _direction;                bool _directionIs;
		vec3    _rightVector;              bool _rightVectorIs;
		vec3    _localUpVector;            bool _localUpVectorIs;
		vec3    _localDirection;           bool _localDirectionIs;
		vec3    _localRightVector;         bool _localRightVectorIs;

		GLfloat		_farValue;
		GLfloat		_nearValue;
		GLfloat		_viewAngle;
		GLint		_windowWidth;
		GLint		_windowHeight;

		GLfloat     _left;
		GLfloat     _right;
		GLfloat     _bottom;
		GLfloat     _top;

		GLfloat		_minPositionOffset;
		GLfloat		_maxPositionOffset;
		GLfloat		_positionOffset;

		AABB        _aabb;                 bool _aabbIs;

		vec3 transformToGlobal(glm::vec3 vector);

		//void updateProjection();
		//void updateView();
};

#endif // __CAMERASTATIC_HPP__
