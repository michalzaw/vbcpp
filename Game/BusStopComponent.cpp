#include "BusStopComponent.h"
#include "../Scene/SceneManager.h"


void BusStopComponent::setAnnouncementFileName(std::string announcementFileName)
{
    _announcementFileName = announcementFileName;

    _announcementSound = ResourceManager::getInstance().loadSound(_announcementFileName);
}
