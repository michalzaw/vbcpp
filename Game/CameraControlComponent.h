#ifndef CAMERAFPSPLAYER_H_INCLUDED
#define CAMERAFPSPLAYER_H_INCLUDED


#include "../Scene/Component.h"


class CameraFPS;


class CameraControlComponent : public Component
{
	private:
		CameraFPS* _camera;

		bool _movmentControl;
		bool _rotationControl;

	public:
		CameraControlComponent(CameraFPS* camera);

		void setMovmentControl(bool isEnable);
		void setRotationControl(bool isEnable);
		bool isMOvementControl();
		bool isRotationControl();

		void update(float deltaTime) override;

};


#endif // CAMERAFPSPLAYER_H_INCLUDED
