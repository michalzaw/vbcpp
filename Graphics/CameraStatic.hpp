#ifndef __CAMERASTATIC_HPP__
#define __CAMERASTATIC_HPP__

#include <cmath>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "../Scene/Component.h"

#include "../Utils/Math.h"

using glm::vec3;
using glm::mat4;

class CameraStatic : public Component
{
	public:
		CameraStatic(int width = 800, int height = 600, GLfloat viewAngle = 45.0f, GLfloat nearValue = 0.1f, GLfloat farValue = 1000.0f);

		virtual ~CameraStatic();

		vec3 getPosition();
		vec3 getLookAtVector();
		vec3 getUpVector();
		vec3 getDirection();
		vec3 getRightVector();

		GLfloat getNearValue();
		GLfloat	getFarValue();
		GLfloat getViewAngle();

		mat4 getProjectionMatrix();
		mat4 getViewMatrix();

		void setFarValue(GLfloat value);
		void setNearValue(GLfloat value);
		void setViewAngle(GLfloat angle);
		void setWindowDimensions(GLint width, GLint height);

		virtual void updateTransform()
		{
		    _viewMatrixIs = false;
		    _lookAtIs = false;
		    _upVectorIs = false;
		    _directionIs = false;
		    _rightVectorIs = false;
		}

	protected:
		mat4	_projectionMatrix;         bool _projectionMatrixIs;
		mat4	_viewMatrix;               bool _viewMatrixIs;

		vec3	_lookAt;                   bool _lookAtIs;
		vec3	_upVector;                 bool _upVectorIs;
		vec3    _direction;                bool _directionIs;
		vec3    _rightVector;              bool _rightVectorIs;

		GLfloat		_farValue;
		GLfloat		_nearValue;
		GLfloat		_viewAngle;
		GLint		_windowWidth;
		GLint		_windowHeight;

		//void updateProjection();
		//void updateView();
};

#endif // __CAMERASTATIC_HPP__
