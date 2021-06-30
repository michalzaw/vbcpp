#ifndef TESTSCENE_H_INCLUDED
#define TESTSCENE_H_INCLUDED


#include "GameScene.h"


class TestScene final : public GameScene
{
	private:
		SceneObject* busObject;

		CameraFPS* createCameraFPSGlobal();

	public:
		explicit TestScene(Window* window);
		~TestScene();

		void initialize() override;

		void fixedStepUpdate(double deltaTime) override;
		void update(double deltaTime) override;


		void readInput(double deltaTime) override;
		void fixedStepReadInput(float deltaTime) override;

		void terminate() override;
};


#endif // TESTSCENE_H_INCLUDED