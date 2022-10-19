#ifndef LOADINGSCREENSCENE_H_INCLUDED
#define LOADINGSCREENSCENE_H_INCLUDED


#include "GameScene.h"


class LoadingScreenScene final : public GameScene
{
	private:
		CameraFPS* createCameraFPSGlobal();

	public:
		explicit LoadingScreenScene(Window* window);
		~LoadingScreenScene();

		void initialize() override;

		void fixedStepUpdate(double deltaTime) override;
		void update(double deltaTime) override;


		void readInput(double deltaTime) override;
		void fixedStepReadInput(float deltaTime) override;

		void terminate() override;
};


#endif // LOADINGSCREENSCENE_H_INCLUDED