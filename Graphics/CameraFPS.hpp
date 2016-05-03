#ifndef __CCAMERAFPS_HPP__
#define __CCAMERAFPS_HPP__

#include "CameraStatic.hpp"


const GLfloat PI = 3.14159265359f;

/*
inline GLfloat DegToRad(GLfloat deg)
{
    return GLfloat((deg * PI) / 180);
}

inline GLfloat RadToDeg(GLfloat rad)
{
    return GLfloat((rad * 180 ) / PI);
}
*/

class CameraFPS : virtual public CameraStatic
{
	public:
		CameraFPS(int width = 800, int height = 600, GLfloat viewAngle = 45.0f, GLfloat nearValue = 0.1f, GLfloat farValue = 1000.0f);

		virtual ~CameraFPS();

		void update();

		void setRotation(int mousePosX, int mousePosY);

		void moveForward(double deltaTime);

		void moveBackward(double deltaTime);

		void strafeLeft(double deltaTime);

		void strafeRight(double deltaTime);

		void setMoveSpeed(float speed);

		void setRotationSpeed(float speed);

		void setCursorPos(GLdouble xpos, GLdouble ypos);

		void reset();

		GLdouble getHorizontalAngle() { return m_HorizontalAngle; }
		GLdouble getVerticalAngle() { return m_VerticalAngle; }

	protected:
		GLdouble m_HorizontalAngle;
		GLdouble m_VerticalAngle;
		GLfloat m_MoveSpeed;
		GLfloat m_RotateSpeed;
		glm::vec3 m_Direction;
		glm::vec3 m_RightVector;

		GLdouble    _oldXpos;
		GLdouble    _oldYpos;

		void updateDirection();
};


#endif // __CCAMERAFPS_HPP__
