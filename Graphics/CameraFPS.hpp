#ifndef __CCAMERAFPS_HPP__
#define __CCAMERAFPS_HPP__

#include "CameraStatic.hpp"




/*

*/

class CameraFPS : virtual public CameraStatic
{
	public:
		CameraFPS(int width = 800, int height = 600, GLfloat viewAngle = 45.0f, GLfloat nearValue = 0.1f, GLfloat farValue = 1000.0f);

		virtual ~CameraFPS();

		//void update();

		void setRotation(int mousePosX, int mousePosY);

		void moveForward(double deltaTime);

		void moveBackward(double deltaTime);

		void strafeLeft(double deltaTime);

		void strafeRight(double deltaTime);

		void setMoveSpeed(float speed);

		void setRotationSpeed(float speed);

		//void setCursorPos(GLdouble xpos, GLdouble ypos);

		void reset();

		//GLdouble getHorizontalAngle() { return m_HorizontalAngle; }
		//GLdouble getVerticalAngle() { return m_VerticalAngle; }

	protected:
		GLfloat m_MoveSpeed;
		GLfloat m_RotateSpeed;

		//GLdouble    _oldXpos;
		//GLdouble    _oldYpos;

		//void updateDirection();
};


#endif // __CCAMERAFPS_HPP__
