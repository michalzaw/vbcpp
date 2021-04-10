#ifndef MENUSELECTBUS_H_INCLUDED
#define MENUSELECTBUS_H_INCLUDED


#include "GameScene.h"


class MenuSelectBusScene final : public GameScene
{
	private:
		std::vector<SceneObject*> _buses;
		std::vector<Bus*> _buses2;
		int _selectedBus;

		CameraFPS* createCameraFPSGlobal();
		void addBus(const std::string& modelFileName, const std::string& texturePath);
		void addBus(const std::string& name);

		void selectNextBus();
		void selectPreviousBus();

	public:
		MenuSelectBusScene(Window* window, PhysicsManager* physicsManager, SoundManager* soundManager, SceneManager* sceneManager, GUIManager* gui);
		~MenuSelectBusScene();

		void initialize() override;

		void fixedStepUpdate(double deltaTime) override;
		void update(double deltaTime) override;


		void readInput(double deltaTime) override;
		void fixedStepReadInput(float deltaTime) override;

		void terminate() override;

};


#endif // MENUSELECTBUS_H_INCLUDED
