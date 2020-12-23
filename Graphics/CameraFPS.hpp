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
		void setMinVerticalAngle(float angle);
		void setMaxVerticalAngle(float angle);

		float getMoveSpeed();
		float getRotationSpeed();
		float getMinVerticalAngle();
		float getMaxVerticalAngle();

		//void setCursorPos(GLdouble xpos, GLdouble ypos);

		void reset();

		//GLdouble getHorizontalAngle() { return m_HorizontalAngle; }
		//GLdouble getVerticalAngle() { return m_VerticalAngle; }

	protected:
		float _moveSpeed;
		float _rotateSpeed;

		float _minVerticalAngle;
		float _maxVerticalAngle;

		//GLdouble    _oldXpos;
		//GLdouble    _oldYpos;

		//void updateDirection();
};


#endif // __CCAMERAFPS_HPP__
