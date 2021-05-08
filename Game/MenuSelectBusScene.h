#ifndef MENUSELECTBUS_H_INCLUDED
#define MENUSELECTBUS_H_INCLUDED


#include "GameScene.h"


struct BusPreview;


class MenuSelectBusScene final : public GameScene
{
	private:
		std::vector<std::string> _availableBusesNames;

		std::vector<SceneObject*> _buses;
		std::vector<BusPreview*> _buses2;
		int _selectedBus;
		int _selectedBusConfiguration;
		Image* _busLogo;

		void loadAvailableBusesNames();

		CameraFPS* createCameraFPSGlobal();
		void addBus(const std::string& modelFileName, const std::string& texturePath);
		void addBus(const std::string& name);

		void selectNextBus();
		void selectPreviousBus();
		void selectNextBusConfiguration();
		void selectPreviousBusConfiguration();

		void showBusLogo();

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
