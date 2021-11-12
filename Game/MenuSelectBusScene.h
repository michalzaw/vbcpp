#ifndef MENUSELECTBUS_H_INCLUDED
#define MENUSELECTBUS_H_INCLUDED


#include "GameScene.h"


struct BusPreview;
class MenuSelectBusInterfaceWindow;


class MenuSelectBusScene final : public GameScene
{
	private:
		std::vector<std::string> _availableBusesNames;

		std::vector<SceneObject*> _buses;
		std::vector<BusPreview*> _buses2;
		int _selectedBus;
		int _selectedBusConfigurationIndex;
		std::unordered_map<std::string, std::string> _selectedBusConfigurationVariables;
		std::string _selectedBusRepaintName;
		Image* _busLogo;

		MenuSelectBusInterfaceWindow* _menuInterfaceWindow;

		RFont* _fontBoldItalic32;
		RFont* _fontRegular22;
		RFont* _fontRegular18;
		float _uiRatio;

		std::vector<GUIObject*> _busLogoWindowElements;
		std::vector<GUIObject*> _busDescriptionWindowElements;
		std::vector<GUIObject*> _busConfigurationWindowElements;
		std::vector<Picker*> _busConfigurationVariablesPickers;
		std::vector<Button*> _busRepaintsButtons;

		void loadAvailableBusesNames();

		CameraFPS* createCameraFPSGlobal();
		void addBus(const std::string& modelFileName, const std::string& texturePath);
		void addBus(const std::string& name);

		void selectNextBus();
		void selectPreviousBus();
		void showSelectedBus();

		void showBusLogo(glm::vec2 windowPosition, int windowWidth, int windowHeight);

		void createBusLogoWindow();
		void createBusDescriptionWindow();
		void createConfigurationWindow();
		void setValuesInVariablesPickers();
		void createConfigurationPreviewWindow();
		void createBusRepaintsWindow();

		void startGame();

	public:
		explicit MenuSelectBusScene(Window* window);
		~MenuSelectBusScene();

		void initialize() override;

		void fixedStepUpdate(double deltaTime) override;
		void update(double deltaTime) override;


		void readInput(double deltaTime) override;
		void fixedStepReadInput(float deltaTime) override;

		void terminate() override;

};


#endif // MENUSELECTBUS_H_INCLUDED
