#include "Hud.h"

#include <iomanip>

#include "../Bus/Bus.h"
#include "../Game/BusStopSystem.h"

#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"


Hud::Hud(GUIManager* gui, Bus* bus)
	: _gui(gui), _bus(bus)
{
	RFont* font = ResourceManager::getInstance().loadFont("C:/Windows/Fonts/ARLRDBD.ttf");

	_hudImage = gui->addImage(ResourceManager::getInstance().loadTexture("Data/HUD/HUD.bmp"));
	_hudImage->setPosition(0, 0);

	_labelSpeed = gui->addLabel(font, "0");
	_labelSpeed->setPosition(20, 122);
	_labelSpeed->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_labelSpeed->scale(0.5f, 0.5f);

	_labelrpm = gui->addLabel(font, "0");
	_labelrpm->setPosition(100, 122);
	_labelrpm->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_labelrpm->scale(0.5f, 0.5f);

	_labelFuel = gui->addLabel(font, "100");
	_labelFuel->setPosition(20, 98);
	_labelFuel->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_labelFuel->scale(0.5f, 0.5f);

	_labelGear = gui->addLabel(font, "N");
	_labelGear->setPosition(100, 96);
	_labelGear->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_labelGear->scale(0.6f, 0.6f);

	_labelPassengers = gui->addLabel(font, "0");
	_labelPassengers->setPosition(20, 44);
	_labelPassengers->setColor(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
	_labelPassengers->scale(0.4f, 0.4f);

	_labelPassengersGettingOff = gui->addLabel(font, "0");
	_labelPassengersGettingOff->setPosition(66, 44);
	_labelPassengersGettingOff->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	_labelPassengersGettingOff->scale(0.4f, 0.4f);

	_imageEngineControl = gui->addImage(ResourceManager::getInstance().loadTexture("Data/HUD/HEN.bmp"));
	_imageEngineControl->setPosition(54, 7);
	_imageEngineControl->setIsActive(false);

	_imageHeadlightsControl = gui->addImage(ResourceManager::getInstance().loadTexture("Data/HUD/HLS.bmp"));
	_imageHeadlightsControl->setPosition(54, 25);
	_imageHeadlightsControl->setIsActive(false);

	_imageStopControl = gui->addImage(ResourceManager::getInstance().loadTexture("Data/HUD/HST.bmp"));
	_imageStopControl->setPosition(98, 25);
	_imageStopControl->setIsActive(false);

	for (int i = 0; i < _bus->getDoorsCount(); ++i)
	{
		Image* imageDoor = gui->addImage(ResourceManager::getInstance().loadTexture("Data/HUD/HDC.bmp"));
		imageDoor->setPosition(115 - i * 15, 62);
		imageDoor->setIsActive(true);

		_imagesDoorClosed.push_back(imageDoor);

		imageDoor = gui->addImage(ResourceManager::getInstance().loadTexture("Data/HUD/HDO.bmp"));
		imageDoor->setPosition(115 - i * 15, 62);
		imageDoor->setIsActive(false);

		_imagesDoorOpened.push_back(imageDoor);
	}

	_labelBusStop = gui->addLabel(font, "");
	_labelBusStop->setPosition(GameConfig::getInstance().windowWidth / 2.0f - 100, GameConfig::getInstance().windowHeight - 50);
	_labelBusStop->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_labelBusStop->scale(0.6f, 0.6f);

	_labelBusStop2 = gui->addLabel(font, "");
	_labelBusStop2->setPosition(GameConfig::getInstance().windowWidth / 2.0f - 100, GameConfig::getInstance().windowHeight - 80);
	_labelBusStop2->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_labelBusStop2->scale(0.6f, 0.6f);

	_labelStats = gui->addLabel(font, "");
	_labelStats->setPosition(200, 10);
	_labelStats->setColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	_labelStats->setScale(0.4f, 0.4f);
}


Hud::~Hud()
{

}


std::string Hud::getGearAsString(int gear)
{
	std::string gearText;
	if (gear == 0)
		return "R";
	else if (gear == 1)
		return "N";
	else
		return toString(gear - 1);
}

std::string Hud::createStatsString(int fps)
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(3) << _bus->getEngine()->getThrottle();
	std::string throttleStr = stream.str();

	std::stringstream stream2;
	stream2 << std::fixed << std::setprecision(3) << _bus->getEngine()->getCurrentTorque();
	std::string torqueStr = stream2.str();

	return "Throttle: " + throttleStr + " Torque: " + torqueStr + " FPS: " +toString(fps);
}


void Hud::update(int fps)
{
	_labelSpeed->setText(toString((int)_bus->getBusSpeed()));
	_labelrpm->setText(toString((int)_bus->getEngine()->getCurrentRPM()));

	int gear = _bus->getGearbox()->currentGear();
	_labelGear->setText(getGearAsString(gear));

	_labelPassengers->setText(toString(_bus->getNumberOfPassengers()));
	_labelPassengersGettingOff->setText(toString(_bus->getNumberOfPassengersGettingOff()));

	_imageEngineControl->setIsActive(_bus->getEngine()->isRunning());
	_imageHeadlightsControl->setIsActive(_bus->isEnableHeadlights());
	_imageStopControl->setIsActive(_bus->getHandbrakeState());

	for (int i = 0; i < _bus->getDoorsCount(); ++i)
	{
		_imagesDoorClosed[i]->setIsActive(_bus->getDoor(i)->getState() == EDS_CLOSING);
		_imagesDoorOpened[i]->setIsActive(_bus->getDoor(i)->getState() == EDS_OPENING);
	}


	BusStopSystem& busStopSystem = BusStopSystem::getInstance();
	if (busStopSystem.getCurrentBusStop() != NULL)
	{
		_labelBusStop->setText(busStopSystem.getCurrentBusStop()->getName() + " (" + toString((int)busStopSystem.getDistanceToCurrentBusStop()) + "m)");
		_labelBusStop2->setText("Liczba pasazerow: " + toString(busStopSystem.getCurrentBusStop()->getNumberOfPassengers()));
	}
	else
	{
		_labelBusStop->setText("");
		_labelBusStop2->setText("");
	}

	_labelStats->setText(createStatsString(fps));
}
