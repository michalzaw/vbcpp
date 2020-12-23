#ifndef CAMERAFPSPLAYER_H_INCLUDED
#define CAMERAFPSPLAYER_H_INCLUDED


#include "../Scene/Component.h"


class CameraFPS;


class CameraControlComponent : public Component
{
	private:
		const short int RAY_TEST_FILTER_MASK;
		const short int RAY_TEST_FILTER_GROUP;

		CameraFPS* _camera;

		bool _movmentControl;
		bool _rotationControl;

		float _changePositionOffsetSpeed;
		float _deltaPositionOffset;

		void updateFromInput(float deltaTime);
		void updateCameraPositionOffset(float deltaTime);
		void moveCameraAboveTerrain();

	public:
		CameraControlComponent(CameraFPS* camera);

		void setMovmentControl(bool isEnable);
		void setRotationControl(bool isEnable);
		void setChangePositionOffsetSpeed(float speed);
		bool isMOvementControl();
		bool isRotationControl();
		float getChangePositionOffsetSpeed();

		void update(float deltaTime) override;

};


#endif // CAMERAFPSPLAYER_H_INCLUDED
