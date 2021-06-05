#include "BusPreviewLoader.h"

#include "BusConfigurationsLoader.h"

#include "../Game/Directories.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/Helpers.hpp"
#include "../Utils/Logger.h"
#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"
#include "../Utils/XmlUtils.h"
#include "../Utils/tinyxml2.h"
#include "../Graphics/DisplayComponent.h"


BusPreviewLoader::BusPreviewLoader(SceneManager* smgr, PhysicsManager* pmgr, SoundManager* sndMgr)
	: BusLoader(smgr, pmgr, sndMgr)
{

}


void BusPreviewLoader::loadAvailableVariables(XMLElement* busElement)
{
    XMLElement* variablesElement = busElement->FirstChildElement("Variables");
    if (variablesElement != nullptr)
    {
        for (XMLElement* variableElement = variablesElement->FirstChildElement("Variable"); variableElement != nullptr; variableElement = variableElement->NextSiblingElement("Variable"))
        {
            GameVariable variable;
            variable.name = XmlUtils::getAttributeString(variableElement, "name");
            variable.defaultValue = XmlUtils::getAttributeStringOptional(variableElement, "defaultValue");
            variable.values = split(XmlUtils::getAttributeStringOptional(variableElement, "values"), ',');
            variable.displayName = XmlUtils::getAttributeStringOptional(variableElement, "displayName", variable.name);
            variable.description = XmlUtils::getAttributeStringOptional(variableElement, "description");
            variable.defaultValue = XmlUtils::getAttributeStringOptional(variableElement, "defaultValue", variable.values[0]);

            _busPreview->availableVariables.push_back(variable);

            _variables[variable.name] = variable.defaultValue;
        }
    }
}


void BusPreviewLoader::loadModuleConditionalElements(XMLElement* moduleElement, XMLElement* busElement, BusRayCastModule& busModule)
{
    for (XMLElement* conditionalElement = moduleElement->FirstChildElement("ConnditionalElements");
        conditionalElement != nullptr;
        conditionalElement = conditionalElement->NextSiblingElement("ConnditionalElements"))
    {
        const std::string variable = XmlUtils::getAttributeString(conditionalElement, "variable");
        const std::string value = XmlUtils::getAttributeString(conditionalElement, "value");

        const auto& gameVariable = std::find_if(begin(_busPreview->availableVariables),
                                                end(_busPreview->availableVariables),
                                                [&variable](GameVariable gameVariable) { return gameVariable.name == variable; });
        if (gameVariable != end(_busPreview->availableVariables))
        {
            SceneObject* sceneObject = _sMgr->addSceneObject(variable);
            SceneObject* busModuleSceneObject = busModule.sceneObject;

            busModuleSceneObject->addChild(sceneObject);

            // chwilowo podmieniam sceneObject w busModule, aby kolejne elementy byly podlaczane pod nowy obiekt zalezny od zmiennej
            busModule.sceneObject = sceneObject;

            loadModuleElements(conditionalElement, busElement, busModule);

            // przywracam prawidlowy sceneObject
            busModule.sceneObject = busModuleSceneObject;

            _busPreview->variableDependentObjects[variable][value].push_back(sceneObject);
        }
        else
        {
            Logger::error("Variable " + variable + " not exist");
        }
    }
}


void BusPreviewLoader::loadWheels(XMLElement* moduleElement, BusRayCastModule& busModule)
{
    for (XMLElement* wheelElement = moduleElement->FirstChildElement("Wheel"); wheelElement != nullptr; wheelElement = wheelElement->NextSiblingElement("Wheel"))
    {
        Logger::info("XML: Wheel data");

        const std::string wheelName(wheelElement->Attribute("name"));
        const std::string wheelModel(wheelElement->Attribute("model"));
        const std::string side(wheelElement->Attribute("side"));
        const float suspensionRestLength = (float)atof(wheelElement->Attribute("suspensionRestLength"));
        const glm::vec3 wheelPosition = XMLstringToVec3(wheelElement->Attribute("position"));

        const float wheelAngle = side == "right" ? 180.0f : 0.0f;

        SceneObject* wheelObj = _sMgr->addSceneObject(wheelName);
        wheelObj->setPosition(wheelPosition - glm::vec3(0.0f, suspensionRestLength, 0.0f));
        wheelObj->setRotation(0.0f, degToRad(wheelAngle), 0.0f);

        busModule.sceneObject->addChild(wheelObj);

        const std::string modelPath = _busPath + wheelModel;
        RStaticModel* wheel = ResourceManager::getInstance().loadModel(modelPath, _texturePath, _normalsSmoothing);
        RenderObject* wheelRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(wheel, true), wheelObj);
    }
}


void BusPreviewLoader::loadDoors(XMLElement* moduleElement, BusRayCastModule& busModule)
{
    for (XMLElement* doorElement = moduleElement->FirstChildElement("Door"); doorElement != nullptr; doorElement = doorElement->NextSiblingElement("Door"))
    {
        Logger::info("XML: Door data");

        const std::string doorName(doorElement->Attribute("name"));
        const std::string doorModelName = XmlUtils::getAttributeStringOptional(doorElement, "model");
        const std::string doorModelNodeName = XmlUtils::getAttributeStringOptional(doorElement, "modelNode");
        const std::string armModelName = XmlUtils::getAttributeStringOptional(doorElement, "armModel");
        const std::string armModelNodeName = XmlUtils::getAttributeStringOptional(doorElement, "armModelNode");

        // door
        glm::vec3 doorPosition;
        glm::vec3 doorRotation;
        RStaticModel* doorModel = nullptr;
        StaticModelNode* doorModelNode = nullptr;
        if (!doorModelName.empty())
        {
            const std::string doorModelPath = _busPath + doorModelName;

            doorModel = ResourceManager::getInstance().loadModel(doorModelPath, _texturePath, _normalsSmoothing);
            doorModelNode = doorModel->getRootNode();

            doorPosition = XmlUtils::getAttributeVec3(doorElement, "position");
            doorRotation = XmlUtils::getAttributeVec3Optional(doorElement, "rotation");

        }
        else if (!doorModelNodeName.empty())
        {
            doorModel = _currentBusModel;
            doorModelNode = doorModel->getNodeByName(doorModelNodeName);

            doorPosition = doorModelNode->transform.getPosition();
            doorRotation = doorModelNode->transform.getRotation();
        }

        if (doorModel != nullptr && doorModelNode != nullptr)
        {
            SceneObject* doorObj = _sMgr->addSceneObject(doorName);
            doorObj->setPosition(doorPosition);
            doorObj->setRotation(doorRotation);

            busModule.sceneObject->addChild(doorObj);

            RenderObject* doorRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(doorModel, doorModelNode, true), doorObj);
        }

        // arm
        glm::vec3 armPosition;
        glm::vec3 armRotation;
        RStaticModel* armModel = nullptr;
        StaticModelNode* armModelNode = nullptr;
        if (!armModelName.empty())
        {
            const std::string armModelPath = _busPath + armModelName;

            armModel = ResourceManager::getInstance().loadModel(armModelPath, _texturePath, _normalsSmoothing);
            armModelNode = armModel->getRootNode();

            armPosition = XmlUtils::getAttributeVec3(doorElement, "armPosition");
            armRotation = XmlUtils::getAttributeVec3Optional(doorElement, "armRotation");
        }
        else if (!armModelNodeName.empty())
        {
            armModel = _currentBusModel;
            armModelNode = armModel->getNodeByName(armModelNodeName);

            armPosition = armModelNode->transform.getPosition();
            armRotation = armModelNode->transform.getRotation();
        }

        if (armModel != nullptr && armModelNode != nullptr)
        {
            SceneObject* armObj = _sMgr->addSceneObject("arm");
            armObj->setPosition(armPosition);
            armObj->setRotation(armRotation);

            busModule.sceneObject->addChild(armObj);

            RenderObject* armRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(armModel, armModelNode, true), armObj);
        }
    }
}


BusPreview* BusPreviewLoader::loadBusPreview(const std::string& busName)
{
    _busPreview = new BusPreview;

    _busPreview->bus = loadBus(busName);

    // ustawienie konfiguracji na podstawie domyslnych wartosci zmiennych
    //_busPreview->setConfiguration(_variables);

    BusConfigurationsLoader::loadAllBusPredefinedConfigurations(busName, _busPreview->predefinedConfigurations, _variables);

    return _busPreview;
}
