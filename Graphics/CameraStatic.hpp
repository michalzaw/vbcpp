#ifndef __CAMERASTATIC_HPP__
#define __CAMERASTATIC_HPP__

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using glm::vec3;
using glm::mat4;

class CameraStatic
{
	public:
		CameraStatic(int width = 800, int height = 600, GLfloat viewAngle = 45.0f, GLfloat nearValue = 0.1f, GLfloat farValue = 1000.0f);

		virtual ~CameraStatic();

		vec3 getPosition();

		GLfloat getNearValue();

		GLfloat	getFarValue();

		vec3 getLookAtVector();

		vec3 getUpVector();

		mat4 getProjectionMatrix();

		mat4 getViewMatrix();

		void setPosition(vec3 position);

		void setPosition(GLfloat x, GLfloat y, GLfloat z);

		void lookAt(vec3 lookAt);

		void setUpVector(vec3 upVector);

		void setFarValue(GLfloat value);

		void setNearValue(GLfloat value);

		void setViewAngle(GLfloat angle);

		void setWindowDimensions(GLint width, GLint height);

	protected:
		mat4	m_ProjectionMatrix;
		mat4	m_ViewMatrix;
		vec3		m_Position;
		vec3		m_LookAt;
		vec3		m_UpVector;
		GLfloat		m_FarValue;
		GLfloat		m_NearValue;
		GLfloat		m_ViewAngle;
		GLint		m_WindowWidth;
		GLint		m_WindowHeight;

		void updateProjection();

		void updateView();
};

#endif // __CAMERASTATIC_HPP__
