#include "BusLoader.h"

#include "../Game/Directories.h"

#include "../Utils/FilesHelper.h"
#include "../Utils/Logger.h"
#include "../Utils/ResourceManager.h"
#include "../Utils/Strings.h"
#include "../Utils/XmlUtils.h"
#include "../Utils/tinyxml2.h"
#include "../Graphics/DisplayComponent.h"


using namespace tinyxml2;


BusLoader::BusLoader(SceneManager* sceneManager, PhysicsManager* physicsManager, SoundManager* soundManager)
    : _sMgr(sceneManager), _pMgr(physicsManager), _sndMgr(soundManager),
    _busCollidesWith(COL_TERRAIN | COL_ENV), _wheelCollidesWith(COL_TERRAIN | COL_ENV), _doorCollidesWith(COL_TERRAIN | COL_ENV),
    _currentBusModel(nullptr)
{

}

Bus* BusLoader::loadBus(const std::string& busName)
{
    std::string configFileName = GameDirectories::BUSES + busName + "/" + BUS_CONFIG_FILENAME;

#ifdef DEVELOPMENT_RESOURCES
	if (!FilesHelper::isFileExists(configFileName))
		configFileName = ResourceManager::getInstance().getAlternativeResourcePath() + configFileName;
#endif // DEVELOPMENT_RESOURCES

    XMLDocument doc;
    doc.LoadFile(configFileName.c_str());

    XMLElement* busElement = doc.FirstChildElement("Bus");

    Logger::info("Bus XML DATA");

    _objName = busElement->Attribute("name");
    std::string textureFolder(busElement->Attribute("textures"));
    std::string busType = XmlUtils::getAttributeStringOptional(busElement, "type", "raycast");
	_normalsSmoothing = XmlUtils::getAttributeBoolOptional(busElement, "normalsSmoothing", true);

    _busPath = GameDirectories::BUSES + busName + "/";
    _texturePath = _busPath + textureFolder + "/";

    Logger::info("Bus name: " + _objName);

    if (busType == "raycast")
    {
        return loadBusRaycast(busElement);
    }
    else if (busType == "constraint")
    {
        return new BusConstraint(_sMgr, _pMgr, _sndMgr, busName);
    }
    else
    {
        Logger::error("Unsupported bus type: " + busType);
        return NULL;
    }
}


BusRaycast* BusLoader::loadBusRaycast(XMLElement* busElement)
{
    _bus = new BusRaycast();

    loadBusDescription(busElement);

    bool result = loadBusModules(busElement);
    if (!result)
    {
        delete _bus;
        return NULL;
    }

    loadEngineAndGearbox(busElement);

    return _bus;
}


void BusLoader::loadEngineAndGearbox(XMLElement* busElement)
{
    XMLElement* objElement = busElement->FirstChildElement("Gearbox");
    const std::string gearboxFile(objElement->Attribute("model"));
    _bus->_gearbox = std::unique_ptr<Gearbox>(new Gearbox(gearboxFile));

    objElement = busElement->FirstChildElement("Engine");
    const std::string engineFile = objElement->Attribute("model");
    const glm::vec3 enginePosition = XmlUtils::getAttributeVec3Optional(objElement, "position");
    const int moduleIndex = XmlUtils::getAttributeIntOptional(objElement, "module");

    _bus->_engine = std::unique_ptr<Engine>(new Engine(engineFile));


    if (moduleIndex < _bus->_modules.size())
    {
        SceneObject* engineSoundsObject = _sMgr->addSceneObject("busEngine");
        engineSoundsObject->setPosition(enginePosition);

        _bus->_modules[moduleIndex].sceneObject->addChild(engineSoundsObject);
        _bus->_engineSoundsObject = engineSoundsObject;

        for (const auto& soundDefinition : _bus->_engine->getEngineLoopedSounds())
        {
            SoundComponent* engineSound = createSound(engineSoundsObject, soundDefinition);
            _bus->_engineLoopedSounds.push_back(engineSound);
        }
        for (int i = 0; i < _bus->_engine->getEngineSounds().size(); ++i)
        {
            SoundComponent* engineSound = createSound(engineSoundsObject, _bus->_engine->getEngineSounds()[i]);
            _bus->_engineSounds.push_back(engineSound);
        }
    }
    else
    {
        Logger::error("Error while loading engine sounds. Invali module index.");
    }
}


void BusLoader::loadBusDescription(XMLElement* busElement)
{
    XMLElement* descriptionElement = busElement->FirstChildElement("Description");
    if (descriptionElement != nullptr)
    {
        _bus->_busDescription.model = XmlUtils::getAttributeStringOptional(descriptionElement, "model");
        _bus->_busDescription.description = XmlUtils::getAttributeStringOptional(descriptionElement, "description");
        _bus->_busDescription.author = XmlUtils::getAttributeStringOptional(descriptionElement, "author");
        _bus->_busDescription.logo = _busPath + XmlUtils::getAttributeStringOptional(descriptionElement, "logo");
    }
}


bool BusLoader::loadBusModules(XMLElement* busElement)
{
    // Reading bus body configuration
    XMLElement* moduleElement = busElement->FirstChildElement("Module");
    while (moduleElement != nullptr)
    {
        std::string modelFile = XmlUtils::getAttributeString(moduleElement, "model");
        glm::vec3 modulePosition = XmlUtils::getAttributeVec3Optional(moduleElement, "position");
        float mass = XmlUtils::getAttributeFloat(moduleElement, "mass");


        BusRayCastModule busModule;
        busModule.sceneObject = _sMgr->addSceneObject(_objName);
        busModule.sceneObject->setPosition(modulePosition);


        // Creating bus render object

        std::string modelPath = _busPath + modelFile;

        std::vector<std::string> nodeToSkip;
        fetchOptionalModelNodes(moduleElement, modelPath, _texturePath, nodeToSkip);

        if (nodeToSkip.empty())
            _currentBusModel = ResourceManager::getInstance().loadModel(modelPath, _texturePath, _normalsSmoothing);
        else
            _currentBusModel = ResourceManager::getInstance().loadModelWithHierarchy(modelPath, _texturePath, _normalsSmoothing);

        RenderObject* busRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(_currentBusModel, nodeToSkip, true), busModule.sceneObject);


        // Tworzenie fizycznego obiektu karoserii

        if (_currentBusModel->getCollisionMeshSize() > 0)
        {
            int wheelCollidesWith = COL_TERRAIN | COL_ENV;

            busModule.rayCastVehicle = _pMgr->createPhysicalBodyRayCastVehicle(_currentBusModel->getCollisionMesh(), _currentBusModel->getCollisionMeshSize(), mass, COL_BUS, _busCollidesWith);
            busModule.rayCastVehicle->setWheelCollisionFilter(COL_WHEEL, _wheelCollidesWith);
            busModule.rayCastVehicle->getRigidBody()->setActivationState(DISABLE_DEACTIVATION);

            busModule.sceneObject->addComponent(busModule.rayCastVehicle);
        }
        else
        {
            Logger::error("Collision mesh not found in bus model!\n");
            return false;
        }


        // Loading other bus module elements

        loadWheels(moduleElement, busModule);
        loadInteriorLights(moduleElement, busModule);
        loadDriverParams(busElement, busModule);
        loadSteeringWheel(moduleElement, busModule);
        loadDesktop(moduleElement, busModule);
        loadHeadlights(moduleElement, busModule);
        loadDoors(moduleElement, busModule);
        loadEnvironmentCaptureComponents(moduleElement, busModule);
        loadMirrors(moduleElement, busModule);
		loadDisplays(moduleElement, busModule);

        _bus->_modules.push_back(busModule);

        moduleElement = moduleElement->NextSiblingElement("Module");
    }

    createRequireSoundComponents();

    // If it's articulated bus - connect modules with a ball joint
    if (_bus->_modules.size() > 1)
    {
        _pMgr->createConstraintBall(_bus->_modules[0].rayCastVehicle, _bus->_modules[1].rayCastVehicle, _bus->_modules[0].jointPosition, _bus->_modules[1].jointPosition);
    }

    if (_bus->_desktop == NULL)
    {
        _bus->_desktop = new Desktop(NULL, NULL);
    }

	_bus->updateDisplays();

    return true;
}


void BusLoader::fetchOptionalModelNodes(XMLElement* moduleElement, std::string modelPath, std::string texturePath, std::vector<std::string>& modelNodesNames)
{
    XMLElement* doorElement = moduleElement->FirstChildElement("Door");
    while (doorElement != nullptr)
    {
        std::string doorModelNodeName = XmlUtils::getAttributeStringOptional(doorElement, "modelNode");
        std::string doorCollisionModelNodeName = XmlUtils::getAttributeStringOptional(doorElement, "collisionModelNode");
        std::string armModelNodeName = XmlUtils::getAttributeStringOptional(doorElement, "armModelNode");

        if (!doorModelNodeName.empty())
            modelNodesNames.push_back(doorModelNodeName);

        if (!doorCollisionModelNodeName.empty())
            modelNodesNames.push_back(doorCollisionModelNodeName);

        if (!armModelNodeName.empty())
            modelNodesNames.push_back(armModelNodeName);

        doorElement = doorElement->NextSiblingElement("Door");
    }
}


void BusLoader::loadWheels(XMLElement* moduleElement, BusRayCastModule& busModule)
{
    XMLElement* wheelElement = moduleElement->FirstChildElement("Wheel");
    if (wheelElement == nullptr)
    {
        Logger::error("Cannot load Wheel elements from config file!");
    }

    while (wheelElement != nullptr)
    {
        Logger::info("XML: Wheel data");

        std::string wheelName(wheelElement->Attribute("name"));
        Logger::info(wheelName);
        std::string wheelModel(wheelElement->Attribute("model"));
        std::string side(wheelElement->Attribute("side"));
        float radius = (float)atof(wheelElement->Attribute("radius"));

        int steering = (int)atoi(wheelElement->Attribute("steering"));
		int tag = XmlUtils::getAttributeIntOptional(wheelElement, "tag", 0);
        int powered = (int)atoi(wheelElement->Attribute("powered"));
        int handbrake = (int)atoi(wheelElement->Attribute("handbrake"));

        float suspensionRestLength = (float)atof(wheelElement->Attribute("suspensionRestLength"));
        float suspensionStiffness = (float)atof(wheelElement->Attribute("suspensionStiffness"));
        float dampingCompression = (float)atof(wheelElement->Attribute("dampingCompression"));
        float dampingRelaxation = (float)atof(wheelElement->Attribute("dampingRelaxation"));
        float frictionSlip = (float)atof(wheelElement->Attribute("frictionSlip"));
        float rollInfluence = (float)atof(wheelElement->Attribute("rollInfluence"));
        float brakeForce = (float)atof(wheelElement->Attribute("brakeForce"));

        glm::vec3 wheelPosition = XMLstringToVec3(wheelElement->Attribute("position"));

        SceneObject* wheelObj = _sMgr->addSceneObject(wheelName);

        // obracamy model kola je¿li jest po lewej stronie
        WheelSide wheelSide;
        float wheelAngle;
        if (side == "right")
        {
            wheelAngle = 0.0f;
            wheelSide = WS_RIGHT;
        }
        else
        {
            wheelAngle = 180.0f;
            wheelSide = WS_LEFT;
        }


        SceneObject* wheelSubObjectForModel = _sMgr->addSceneObject(wheelName + "Model");
        wheelSubObjectForModel->setRotation(0.0f, degToRad(wheelAngle), 0.0f);
        wheelObj->addChild(wheelSubObjectForModel);

        std::string modelPath = _busPath + wheelModel;
        RStaticModel* wheel = ResourceManager::getInstance().loadModel(modelPath, _texturePath, _normalsSmoothing);
        RenderObject* wheelRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(wheel), wheelSubObjectForModel);
		wheelRenderObject->setIsDynamicObject(true);


        btVector3 btWheelPos(wheelPosition.x, wheelPosition.y, wheelPosition.z);

        PhysicalBodyWheel* wheelBody = _pMgr->createPhysicalBodyWheel(busModule.rayCastVehicle, btWheelPos, suspensionRestLength, radius, steering);
        wheelObj->addComponent(wheelBody);

        btWheelInfo& wheelInfo = wheelBody->getWheelInfo();
        wheelInfo.m_suspensionStiffness = suspensionStiffness;
        wheelInfo.m_wheelsDampingCompression = dampingCompression * 2 * sqrt(wheelInfo.m_suspensionStiffness);
        wheelInfo.m_wheelsDampingRelaxation = dampingRelaxation * 2 * sqrt(wheelInfo.m_suspensionStiffness);
        wheelInfo.m_frictionSlip = frictionSlip;
        wheelInfo.m_rollInfluence = rollInfluence;

        BusRayCastWheel* w = new BusRayCastWheel;
        w->wheel = wheelBody;
        w->steering = steering;
		w->tag = tag;
        w->powered = powered;
        w->handbrake = handbrake;
        w->maxBrakeForce = brakeForce;
        w->wheelSide = wheelSide;

        _bus->_wheels.push_back(w);

        wheelElement = wheelElement->NextSiblingElement("Wheel");
    }
}


void BusLoader::loadInteriorLights(XMLElement* moduleElement, BusRayCastModule& busModule)
{
    XMLElement* lightElement = moduleElement->FirstChildElement("Light");
    while (lightElement != nullptr)
    {
        Logger::info("XML: Interior light data");

        glm::vec3 position = XMLstringToVec3(lightElement->Attribute("position"));
        glm::vec3 color = XMLstringToVec3(lightElement->Attribute("color"));
        glm::vec3 attenuation = XMLstringToVec3(lightElement->Attribute("attenuation"));

        float ambientIntensity = (float)atof(lightElement->Attribute("ambientIntensity"));
        float diffuseIntensity = (float)atof(lightElement->Attribute("diffuseIntensity"));


        SceneObject* light = _sMgr->addSceneObject("busLight" + toString(_bus->_lights.size()));
        Light* lightComponent = GraphicsManager::getInstance().addPointLight(color, ambientIntensity, diffuseIntensity,
                                                                             LightAttenuation(attenuation.x, attenuation.y, attenuation.z));
        light->addComponent(lightComponent);
        light->setPosition(position);
        busModule.sceneObject->addChild(light);

        lightComponent->setIsActive(_bus->_isEnableLights);

        _bus->_lights.push_back(lightComponent);

        lightElement = lightElement->NextSiblingElement("Light");
    }
}


void BusLoader::loadDriverParams(XMLElement* busElement, BusRayCastModule& busModule)
{
    XMLElement* driverPositionElement = busElement->FirstChildElement("Driver");
    if (driverPositionElement != nullptr)
    {
        _bus->_driverPosition = XMLstringToVec3(driverPositionElement->Attribute("position"));

        driverPositionElement = driverPositionElement->NextSiblingElement("Driver");
    }
    else
    {
        Logger::warning("Cannot load driver position from config file!");
    }
}


void BusLoader::loadSteeringWheel(XMLElement* moduleElement, BusRayCastModule& busModule)
{
    XMLElement* steeringWheelElement = moduleElement->FirstChildElement("SteeringWheel");
    if (steeringWheelElement != nullptr)
    {
        Logger::info("XML: Steering wheel data");

        std::string modelFile = std::string(steeringWheelElement->Attribute("model"));
        glm::vec3 position = XMLstringToVec3(steeringWheelElement->Attribute("position"));
        glm::vec3 rotation = XMLstringToVec3(steeringWheelElement->Attribute("rotation"));
        glm::vec3 scale = XMLstringToVec3(steeringWheelElement->Attribute("scale"));


        SceneObject* steeringWheelObject = _sMgr->addSceneObject("steeringWheel");
        steeringWheelObject->setPosition(position);
        //steeringWheelObject->setRotation(glm::vec3(rotation.x * PI, rotation.y * PI, rotation.z * PI));
		steeringWheelObject->setRotation(glm::vec3(degToRad(rotation.x), degToRad(rotation.y), degToRad(rotation.z)));
        steeringWheelObject->setScale(scale);

        std::string modelPath = _busPath + modelFile;
        RStaticModel* steeringWheelModel = ResourceManager::getInstance().loadModel(modelPath, _texturePath, _normalsSmoothing);
        RenderObject* renderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(steeringWheelModel), steeringWheelObject);
		renderObject->setIsDynamicObject(true);

        busModule.sceneObject->addChild(steeringWheelObject);

        _bus->_steeringWheelObject = steeringWheelObject;
    }
    else
    {
        Logger::info("Cannot load steering wheel element from config file!");
    }
}


void bindButtonActionToState(BusRaycast* bus, DesktopButtonState& buttonState, DesktopButtonAction action, int actionParam)
{
    buttonState.actionParam = actionParam;

    if (action == DBA_DOOR_OPEN)
    {
        buttonState.actionWithParamInt = std::bind(&BusRaycast::doorOpen, bus, std::placeholders::_1);
    }
    else if (action == DBA_DOOR_CLOSE)
    {
        buttonState.actionWithParamInt = std::bind(&BusRaycast::doorClose, bus, std::placeholders::_1);
    }
    else if (action == DBA_DOOR_OPEN_CLOSE)
    {
        buttonState.actionWithParamInt = std::bind(&BusRaycast::doorOpenClose, bus, std::placeholders::_1);
    }
    else if (action == DBA_DOOR_GROUP_OPEN)
    {
        buttonState.actionWithParamInt = std::bind(&BusRaycast::doorGroupOpen, bus, std::placeholders::_1);
    }
    else if (action == DBA_DOOR_GROUP_CLOSE)
    {
        buttonState.actionWithParamInt = std::bind(&BusRaycast::doorGroupClose, bus, std::placeholders::_1);
    }
    else if (action == DBA_DOOR_GROUP_OPEN_CLOSE)
    {
        buttonState.actionWithParamInt = std::bind(&BusRaycast::doorGroupOpenClose, bus, std::placeholders::_1);
    }
    else if (action == DBA_DOOR_BLOCK)
    {
        buttonState.actionWithParamInt = std::bind(&BusRaycast::doorBlock, bus, std::placeholders::_1);
    }
    else if (action == DBA_DOOR_UNBLOCK)
    {
        buttonState.actionWithParamInt = std::bind(&BusRaycast::doorUnblock, bus, std::placeholders::_1);
    }
    else if (action == DBA_TOGGLE_HANDBRAKE)
    {
        buttonState.action = std::bind(&BusRaycast::toggleHandbrake, bus);
    }
}


void BusLoader::loadDesktop(XMLElement* moduleElement, BusRayCastModule& busModule)
{
    XMLElement* desktopElement = moduleElement->FirstChildElement("Desktop");
    if (desktopElement != nullptr)
    {
        Logger::info("XML: Desktop data");

        std::string modelFile = std::string(desktopElement->Attribute("model"));
        glm::vec3 position = XMLstringToVec3(desktopElement->Attribute("position"));
        glm::vec3 rotation = XMLstringToVec3(desktopElement->Attribute("rotation"));
        glm::vec3 scale = XMLstringToVec3(desktopElement->Attribute("scale"));


        SceneObject* desktopObject = _sMgr->addSceneObject("desktop");
        desktopObject->setPosition(position);
        desktopObject->setRotation(glm::vec3(rotation.x * PI, rotation.y * PI, rotation.z * PI) );
        desktopObject->setScale(scale);

        _bus->_desktopClickableObject = GraphicsManager::getInstance().addClickableObject();
        desktopObject->addComponent(_bus->_desktopClickableObject);

        std::string modelPath = _busPath + modelFile;
        RStaticModel* desktopModel = ResourceManager::getInstance().loadModelWithHierarchy(modelPath, _texturePath, _normalsSmoothing);
        _bus->_desktopRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(desktopModel), desktopObject);
		_bus->_desktopRenderObject->setIsDynamicObject(true);


        busModule.sceneObject->addChild(desktopObject);

        _bus->_desktopObject = desktopObject;
        _bus->_desktop = new Desktop(_bus->_desktopRenderObject, _bus->_desktopClickableObject);

        XMLElement* indicatorElement = desktopElement->FirstChildElement("Indicator");
        while (indicatorElement != nullptr)
        {
            std::string modelNodeName(indicatorElement->Attribute("modelNodeName"));
            std::string variableName(indicatorElement->Attribute("variable"));
            float maxAngle = XmlUtils::getAttributeFloatOptional(indicatorElement, "maxAngle");
            float maxValue = XmlUtils::getAttributeFloatOptional(indicatorElement, "maxValue");
            float minValue = XmlUtils::getAttributeFloatOptional(indicatorElement, "minValue");
            float minAngle = XmlUtils::getAttributeFloatOptional(indicatorElement, "minAngle");
            glm::vec3 axis = XmlUtils::getAttributeVec3Optional(indicatorElement, "rotationAxis", glm::vec3(0.0f, 0.0f, 1.0f));

            std::vector<glm::vec2> curve;
            XmlUtils::loadCurveFromXmlFile(indicatorElement, curve, "value", "angle");
            if (curve.size() > 1)
            {
                for (auto& point : curve)
                {
                    point.y = degToRad(point.y);
                }
                _bus->_desktop->addIndicator(modelNodeName, variableName, curve, axis);
            }
            else
            {
                _bus->_desktop->addIndicator(modelNodeName, variableName, degToRad(maxAngle), maxValue, minValue, degToRad(minAngle), axis);
            }

            indicatorElement = indicatorElement->NextSiblingElement("Indicator");
        }

        XMLElement* buttonElement = desktopElement->FirstChildElement("Button");
        while (buttonElement != nullptr)
        {
            std::string modelNodeName(buttonElement->Attribute("modelNodeName"));
            bool isReturning = XmlUtils::getAttributeBool(buttonElement, "isReturning");
            glm::vec3 rotationAxis = XmlUtils::getAttributeVec3Optional(buttonElement, "rotationAxis", glm::vec3(0.0f, 0.0f, 1.0f));
            bool defaultState = XmlUtils::getAttributeBoolOptional(buttonElement, "defaultState", true);

            std::vector<DesktopButtonState> buttonStates;

            if (defaultState)
            {
                buttonStates.push_back(DesktopButtonState());
            }

            XMLElement* buttonStateElement = buttonElement->FirstChildElement("State");
            while (buttonStateElement != nullptr)
            {
                glm::vec3 translationDirection = XmlUtils::getAttributeVec3Optional(buttonStateElement, "translationDirection");
                float offset = XmlUtils::getAttributeFloatOptional(buttonStateElement, "translationOffset");

                float rotation = XmlUtils::getAttributeFloatOptional(buttonStateElement, "rotation");

                DesktopButtonState state(translationDirection * offset, degToRad(rotation));

                std::string actionName = XmlUtils::getAttributeStringOptional(buttonStateElement, "action");
                if (!actionName.empty())
                {
                    int actionParam = XmlUtils::getAttributeIntOptional(buttonStateElement, "actionParam");

                    DesktopButtonAction action = getDesktopButtonActionFromString(actionName);
                    bindButtonActionToState(_bus, state, action, actionParam);
                }

                buttonStates.push_back(state);

                buttonStateElement = buttonStateElement->NextSiblingElement("State");
            }

            _bus->_desktop->addButton(modelNodeName, rotationAxis, buttonStates, isReturning);

            buttonElement = buttonElement->NextSiblingElement("Button");
        }

        XMLElement* lightElement = desktopElement->FirstChildElement("Light");
        while (lightElement != nullptr)
        {
            const char* cType = lightElement->Attribute("type");
            DesktopLightType type = getDesktopLightTypeFromString(cType);

            std::string modelNodeName(lightElement->Attribute("modelNodeName"));
            glm::vec3 backlightingColor = XMLstringToVec3(lightElement->Attribute("backlightingColor"));
            glm::vec3 lightColor = XMLstringToVec3(lightElement->Attribute("lightColor"));

            _bus->_desktop->setLight(type, modelNodeName, backlightingColor, lightColor);

            lightElement = lightElement->NextSiblingElement("Light");
        }
    }
    else
    {
        Logger::info("Cannot load desktop element from config file!");
    }
}


void BusLoader::loadHeadlights(XMLElement* moduleElement, BusRayCastModule& busModule)
{
    XMLElement* headlightElement = moduleElement->FirstChildElement("Headlight");
    while (headlightElement != nullptr)
    {
        Logger::info("XML: Headlights data");

        std::string headlightName(headlightElement->Attribute("name"));

        glm::vec3 position = XMLstringToVec3(headlightElement->Attribute("position"));
        glm::vec3 rotation = XMLstringToVec3(headlightElement->Attribute("rotation"));

        glm::vec3 color = XMLstringToVec3(headlightElement->Attribute("color"));
        glm::vec3 attenuation = XMLstringToVec3(headlightElement->Attribute("attenuation"));
        float ambientIntensity = (float)atof(headlightElement->Attribute("ambientIntensity"));
        float diffuseIntensity = (float)atof(headlightElement->Attribute("diffuseIntensity"));
        float cutoff = (float)atof(headlightElement->Attribute("cutoff"));


        SceneObject* light = _sMgr->addSceneObject(headlightName);
        Light* lightComponent = GraphicsManager::getInstance().addSpotLight(color, ambientIntensity, diffuseIntensity, cutoff,
                                                                            LightAttenuation(attenuation.x, attenuation.y, attenuation.z));
        light->addComponent(lightComponent);
        light->setPosition(position);
        light->setRotation(rotation);
        busModule.sceneObject->addChild(light);

        lightComponent->setIsActive(_bus->_isEnableHeadlights);

        _bus->_headlights.push_back(lightComponent);

        headlightElement = headlightElement->NextSiblingElement("Headlight");
    }
}


void BusLoader::loadDoors(XMLElement* moduleElement, BusRayCastModule& busModule)
{
    XMLElement* doorElement = moduleElement->FirstChildElement("Door");
    while (doorElement != nullptr)
    {
        Logger::info("XML: Door data");

        // Loading common parameters

        std::string doorName(doorElement->Attribute("name"));
        std::string doorType(doorElement->Attribute("type"));


        bool isDoorLoadedFromSeparateModel = true;

        std::string doorModelName = XmlUtils::getAttributeStringOptional(doorElement, "model");
        std::string doorModelNodeName;
        std::string doorCollisionModelNode;
        if (doorModelName.empty())
        {
            isDoorLoadedFromSeparateModel = false;
            doorModelNodeName = XmlUtils::getAttributeString(doorElement, "modelNode");
            doorCollisionModelNode = XmlUtils::getAttributeString(doorElement, "collisionModelNode");
        }


        glm::vec3 doorPosition;
        glm::vec3 doorRotation;
        RStaticModel* doorModel;
        StaticModelNode* doorModelNode;
        StaticModelNode* doorCollisionNode;
        if (isDoorLoadedFromSeparateModel)
        {
            std::string doorModelPath = _busPath + doorModelName;

            doorModel = ResourceManager::getInstance().loadModel(doorModelPath, _texturePath, _normalsSmoothing);
            doorModelNode = doorModel->getRootNode();
            doorCollisionNode = nullptr; // tymczasowe - zamiast tego bedzie brany collisionMesh utworzony na podstawie materialu

            doorPosition = XmlUtils::getAttributeVec3(doorElement, "position");
            doorRotation = XmlUtils::getAttributeVec3Optional(doorElement, "rotation");
        }
        else
        {
            doorModel = _currentBusModel;
            doorModelNode = doorModel->getNodeByName(doorModelNodeName);
            doorCollisionNode = doorModel->getNodeByName(doorCollisionModelNode);

            doorPosition = doorModelNode->transform.getPosition();
            doorRotation = doorModelNode->transform.getRotation();
        }


        float mass = (float)atof(doorElement->Attribute("mass"));
        char group = (char)atoi(doorElement->Attribute("group"));

        std::string openSound = _busPath + std::string(doorElement->Attribute("doorOpenSound"));
        std::string closeSound = _busPath + std::string(doorElement->Attribute("doorCloseSound"));


        // Creating scene object
        SceneObject* doorObj = _sMgr->addSceneObject(doorName);
        doorObj->setPosition(doorPosition);
        doorObj->setRotation(doorRotation);


        // Create sound component
        RSound* openSoundResource = ResourceManager::getInstance().loadSound(openSound);
        SoundComponent* openSoundComp = new SoundComponent(openSoundResource, EST_PLAYER);
        _sndMgr->addSoundComponent(openSoundComp);

        RSound* closeSoundResource = ResourceManager::getInstance().loadSound(closeSound);
        SoundComponent* closeSoundComp = new SoundComponent(closeSoundResource, EST_PLAYER);
        _sndMgr->addSoundComponent(closeSoundComp);

        doorObj->addComponent(openSoundComp);
        doorObj->addComponent(closeSoundComp);


        if (doorType == "s")
        {
            loadDoorSimple(doorElement, busModule, doorObj, doorModel, mass, group, openSoundComp, closeSoundComp);
        }
        else if (doorType == "se")
        {
            loadDoorSE(doorElement, busModule, doorObj, doorModel, mass, group, openSoundComp, closeSoundComp);
        }
        else if (doorType == "classic")
        {
            loadDoorClassic(doorElement, busModule, doorObj, doorModel, doorModelNode, doorCollisionNode, mass, group, openSoundComp, closeSoundComp, isDoorLoadedFromSeparateModel);
        }
        else
        {
            Logger::warning("Unsupported door type: " + doorType + ". Ignored.");
        }


        doorElement = doorElement->NextSiblingElement("Door");
    }
}


void BusLoader::loadDoorSimple(XMLElement* doorElement, BusRayCastModule& busModule, SceneObject* doorObj, RStaticModel* doorModel, float mass, char group,
                               SoundComponent* openSoundComp, SoundComponent* closeSoundComp)
{
    btVector3 busPivot = XMLstringToBtVec3(doorElement->Attribute("pivotA"));
    btVector3 doorPivot = XMLstringToBtVec3(doorElement->Attribute("pivotB"));
	btVector3 axisA = XmlUtils::getAttributeBtVector3Optional(doorElement, "axisA", btVector3(0, 1, 0));
	btVector3 axisB = XmlUtils::getAttributeBtVector3Optional(doorElement, "axisB", btVector3(0, 1, 0));


    RenderObject* doorRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(doorModel), doorObj);
	doorRenderObject->setIsDynamicObject(true);

    PhysicalBodyConvexHull* doorBody = _pMgr->createPhysicalBodyConvexHull(doorModel->getCollisionMesh(), doorModel->getCollisionMeshSize(), mass, COL_DOOR, _doorCollidesWith);
    doorObj->addComponent(doorBody);

    ConstraintHinge* doorHinge = _pMgr->createConstraintHinge(busModule.rayCastVehicle, doorBody, busPivot, doorPivot, axisA, axisB);

    doorHinge->getBulletConstraint()->setLimit(-1.5,0);

    Door* d = new DoorSimple(doorObj, doorHinge, openSoundComp, closeSoundComp, group);
    d->close();
    _bus->_doors.push_back(d);
}


void BusLoader::loadDoorSE(XMLElement* doorElement, BusRayCastModule& busModule, SceneObject* doorObj, RStaticModel* doorModel, float mass, char group,
                           SoundComponent* openSoundComp, SoundComponent* closeSoundComp)
{
    std::string armName(doorElement->Attribute("armName"));
    std::string armModel(doorElement->Attribute("arm"));
    float armMass = (float)atof(doorElement->Attribute("armMass"));

    glm::vec2 arm1limits = XMLstringToVec2(doorElement->Attribute("arm1limits"));

    std::string rotDir(doorElement->Attribute("rotationDir"));

    RotationDir rdir;

    if (rotDir == "CCW")
        rdir = ERD_CCW;
    else
        rdir = ERD_CW;

    glm::vec3 armPosition = XMLstringToVec3(doorElement->Attribute("armPosition"));
    glm::vec3 armRelPos = glm::vec3(armPosition.x, armPosition.y, armPosition.z);

    btVector3 armPivotA = XMLstringToBtVec3(doorElement->Attribute("armPivotA"));
    btVector3 armPivotB = XMLstringToBtVec3(doorElement->Attribute("armPivotB"));

    SceneObject* armObj = _sMgr->addSceneObject(armName);
    armObj->setPosition(armRelPos);

    std::string armPath = _busPath + armModel;

    RStaticModel* arm = ResourceManager::getInstance().loadModel(armPath, _texturePath, _normalsSmoothing);
    RenderObject* armRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(arm), armObj);
	armRenderObject->setIsDynamicObject(true);

    btVector3 btArmPos(armRelPos.x, armRelPos.y, armRelPos.z);

    PhysicalBodyConvexHull* armBody = _pMgr->createPhysicalBodyConvexHull(arm->getCollisionMesh(), arm->getCollisionMeshSize(), armMass, COL_DOOR, _doorCollidesWith);
    armObj->addComponent(armBody);

	// constraint1
    ConstraintHinge* busToArm1Hinge = _pMgr->createConstraintHinge(busModule.rayCastVehicle, armBody, armPivotA, armPivotB, btVector3(0,1,0), btVector3(0,1,0));

	busToArm1Hinge->getBulletConstraint()->setLimit(arm1limits.x,arm1limits.y);

    // arm 2

    std::string arm2Name(doorElement->Attribute("arm2Name"));
    std::string arm2Model(doorElement->Attribute("arm2"));
    float arm2Mass = (float)atof(doorElement->Attribute("arm2Mass"));

    glm::vec3 arm2Position = XMLstringToVec3(doorElement->Attribute("arm2Position"));
    glm::vec3 arm2RelPos = glm::vec3(arm2Position.x, arm2Position.y, arm2Position.z);

    btVector3 arm2PivotA = XMLstringToBtVec3(doorElement->Attribute("arm2PivotA"));
    btVector3 arm2PivotB = XMLstringToBtVec3(doorElement->Attribute("arm2PivotB"));

    SceneObject* arm2Obj = _sMgr->addSceneObject(arm2Name);
    arm2Obj->setPosition(arm2RelPos);

    std::string arm2Path = _busPath + arm2Model;

    RStaticModel* arm2 = ResourceManager::getInstance().loadModel(arm2Path, _texturePath, _normalsSmoothing);
    RenderObject* arm2RenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(arm2), arm2Obj);
	arm2RenderObject->setIsDynamicObject(true);

    btVector3 btArm2Pos(arm2RelPos.x, arm2RelPos.y, arm2RelPos.z);

    short collidesWith = COL_NOTHING;
    PhysicalBodyConvexHull* arm2Body = _pMgr->createPhysicalBodyConvexHull(arm2->getCollisionMesh(), arm2->getCollisionMeshSize(), arm2Mass, COL_DOOR, collidesWith);
    arm2Obj->addComponent(arm2Body);

    ConstraintHinge* busToArm2Hinge = _pMgr->createConstraintHinge(busModule.rayCastVehicle, arm2Body, arm2PivotA, arm2PivotB, btVector3(0,1,0), btVector3(0,1,0));

    // door model

    //glm::vec3 relativePos = glm::vec3(armPosition.x + doorPosition.x, armPosition.y + doorPosition.y, armPosition.z + doorPosition.z);

    btVector3 doorPivotA = XMLstringToBtVec3(doorElement->Attribute("pivotA"));
    btVector3 doorPivotB = XMLstringToBtVec3(doorElement->Attribute("pivotB"));

    RenderObject* doorRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(doorModel), doorObj);
	doorRenderObject->setIsDynamicObject(true);

    //btVector3 btDoorPos(relativePos.x, relativePos.y, relativePos.z);

    PhysicalBodyConvexHull* doorBody = _pMgr->createPhysicalBodyConvexHull(doorModel->getCollisionMesh(), doorModel->getCollisionMeshSize(), mass, COL_DOOR, _doorCollidesWith);
    doorObj->addComponent(doorBody);

    ConstraintHinge* arm1ToDoorHinge = _pMgr->createConstraintHinge(armBody, doorBody, doorPivotA, doorPivotB, btVector3(0,1,0), btVector3(0,1,0));

    if (rotDir == "CCW")
		arm1ToDoorHinge->getBulletConstraint()->setLimit(-1.9,0.0);
    else
		arm1ToDoorHinge->getBulletConstraint()->setLimit(0.0,1.9);

    btVector3 pivotC = XMLstringToBtVec3(doorElement->Attribute("pivotC"));
    btVector3 pivotD = XMLstringToBtVec3(doorElement->Attribute("pivotD"));

	ConstraintHinge* arm2ToDoorHinge = _pMgr->createConstraintHinge(arm2Body, doorBody, pivotC, pivotD, btVector3(0,1,0), btVector3(0,1,0));


	Door* d = new DoorSE(doorObj, armObj, arm2Obj, busToArm1Hinge, busToArm2Hinge, arm1ToDoorHinge, arm2ToDoorHinge, openSoundComp, closeSoundComp, rdir, group);
    d->close();
    _bus->_doors.push_back(d);
}


void BusLoader::loadDoorClassic(XMLElement* doorElement, BusRayCastModule& busModule, SceneObject* doorObj, RStaticModel* doorModel, StaticModelNode* doorModelNode, StaticModelNode* doorCollisionNode,
                                float mass, char group, SoundComponent* openSoundComp, SoundComponent* closeSoundComp, bool isDoorLoadedFromSeparateModel)
{
    float velocity = XmlUtils::getAttributeFloatOptional(doorElement, "velocity", 1.0f);
    std::string rotDir = XmlUtils::getAttributeStringOptional(doorElement, "rotationDir", "CW");
    RotationDir rotationDirection;

    if (rotDir == "CCW")
        rotationDirection = ERD_CCW;
    else
        rotationDirection = ERD_CW;


    // arm
    float armMass = (float)atof(doorElement->Attribute("armMass"));


    bool isArmLoadedFromSeparateMode = true;

    std::string armModelName = XmlUtils::getAttributeStringOptional(doorElement, "armModel");
    std::string armModelNodeName;
    std::string armCollisionModelNode;
    if (armModelName.empty())
    {
        isArmLoadedFromSeparateMode = false;
        armModelNodeName = XmlUtils::getAttributeString(doorElement, "armModelNode");
        armCollisionModelNode = XmlUtils::getAttributeStringOptional(doorElement, "armCollisionModelNode");
    }


    glm::vec3 armPosition;
    glm::vec3 armRotation;
    RStaticModel* armModel;
    StaticModelNode* armModelNode;
    StaticModelNode* armCollisionNode;
    if (isArmLoadedFromSeparateMode)
    {
        std::string armModelPath = _busPath + armModelName;

        armModel = ResourceManager::getInstance().loadModel(armModelPath, _texturePath, _normalsSmoothing);
        armModelNode = armModel->getRootNode();
        armCollisionNode = nullptr; // tymczasowe - zamiast tego bedzie brany collisionMesh utworzony na podstawie materialu

        armPosition = XmlUtils::getAttributeVec3(doorElement, "armPosition");
        armRotation = XmlUtils::getAttributeVec3(doorElement, "armRotation");
    }
    else
    {
        armModel = _currentBusModel;
        armModelNode = armModel->getNodeByName(armModelNodeName);
        armCollisionNode = armModel->getNodeByName(armCollisionModelNode);

        armPosition = armModelNode->transform.getPosition();
        armRotation = armModelNode->transform.getRotation();
    }


    SceneObject* armObject = _sMgr->addSceneObject("arm");
    armObject->setPosition(armPosition);
    armObject->setRotation(armRotation);

    RenderObject* armRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(armModel, armModelNode), armObject);
	armRenderObject->setIsDynamicObject(true);

    PhysicalBody* armBody;

    if (isArmLoadedFromSeparateMode)
    {
        glm::vec3* armCollisionMesh = armModel->getCollisionMesh();
        unsigned int armCollisionMeshSize = armModel->getCollisionMeshSize();

        armBody = _pMgr->createPhysicalBodyConvexHull(armCollisionMesh, armCollisionMeshSize, mass, COL_DOOR, _doorCollidesWith);
    }
    else if (armCollisionNode != nullptr)
    {
        std::vector<glm::vec3> armCollisionMeshVector;
        armCollisionNode->getVerticesArray(armCollisionMeshVector);

        armBody = _pMgr->createPhysicalBodyConvexHull(armCollisionMeshVector, mass, COL_DOOR, _doorCollidesWith);
    }
    else
    {
        armBody = _pMgr->createPhysicalBodyConvexHull(std::vector<glm::vec3>{}, armMass, COL_DOOR, _doorCollidesWith);
        //armBody = _pMgr->createPhysicalBodyConvexHull((glm::vec3*)foo, 0, mass, COL_DOOR, _doorCollidesWith);
        //armBody = _pMgr->createPhysicalBodyCylinder(btVector3(0.1, 2.0, 0.1), mass, Y_AXIS, COL_DOOR, _doorCollidesWith);
        //armBody = _pMgr->createPhysicalBodyBox(btVector3(0.3, 1.0, 0.3), mass, COL_DOOR, _doorCollidesWith);
        //armBody = _pMgr->createPhysicalBodyConvexHull(armModel->getCollisionMesh(), armModel->getCollisionMeshSize(), armMass, COL_DOOR, COL_NOTHING);
    }

    armObject->addComponent(armBody);


    // door object
    RenderObject* doorRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(doorModel, doorModelNode), doorObj);
	doorRenderObject->setIsDynamicObject(true);

    PhysicalBodyConvexHull* doorBody;
    if (doorCollisionNode == nullptr)
    {
        glm::vec3* doorCollisionMesh = doorModel->getCollisionMesh();
        unsigned int doorCollisionMeshSize = doorModel->getCollisionMeshSize();

        doorBody = _pMgr->createPhysicalBodyConvexHull(doorCollisionMesh, doorCollisionMeshSize, mass, COL_DOOR, _doorCollidesWith);
    }
    else
    {
        std::vector<glm::vec3> doorCollisionMeshVector;
        doorCollisionNode->getVerticesArray(doorCollisionMeshVector);

        doorBody = _pMgr->createPhysicalBodyConvexHull(doorCollisionMeshVector, mass, COL_DOOR, _doorCollidesWith);
    }

    doorObj->addComponent(doorBody);


    // constraints
    btVector3 busToArmPivotA;
    btVector3 busToArmPivotB;
    if (!isDoorLoadedFromSeparateModel)
    {
        busToArmPivotA = btVector3(armPosition.x, armPosition.y, armPosition.z);
        busToArmPivotB = btVector3(0,0,0);
    }
    else
    {
        busToArmPivotA = XmlUtils::getAttributeBtVector3(doorElement, "busToArmPivotA");
        busToArmPivotB = XmlUtils::getAttributeBtVector3(doorElement, "busToArmPivotB");
    }
    btVector3 busToArmAxisA = XmlUtils::getAttributeBtVector3Optional(doorElement, "busToArmAxisA", btVector3(0,1,0));
    btVector3 busToArmAxisB = XmlUtils::getAttributeBtVector3Optional(doorElement, "busToArmAxisB", btVector3(0,1,0));

    btVector3 armToDoorPivotA = XmlUtils::getAttributeBtVector3(doorElement, "armToDoorPivotA");
    btVector3 armToDoorPivotB = XmlUtils::getAttributeBtVector3(doorElement, "armToDoorPivotB");
    btVector3 armToDoorAxisA = XmlUtils::getAttributeBtVector3Optional(doorElement, "armToDoorAxisA", btVector3(0,1,0));
    btVector3 armToDoorAxisB = XmlUtils::getAttributeBtVector3Optional(doorElement, "armToDoorAxisB", btVector3(0,1,0));


    ConstraintHinge* hingeBusToArm = _pMgr->createConstraintHinge(busModule.rayCastVehicle, armBody, busToArmPivotA, busToArmPivotB, busToArmAxisA, busToArmAxisB);
    ConstraintHinge* hingeArmToDoor = _pMgr->createConstraintHinge(armBody, doorBody, armToDoorPivotA, armToDoorPivotB, armToDoorAxisA, armToDoorAxisB);

    if (rotationDirection == ERD_CW)
    {
        hingeBusToArm->getBulletConstraint()->setLimit(0, 1.5);
        hingeArmToDoor->getBulletConstraint()->setLimit(-3.14,0);
    }
    else if (rotationDirection == ERD_CCW)
    {
        hingeBusToArm->getBulletConstraint()->setLimit(-1.5 ,0);
        hingeArmToDoor->getBulletConstraint()->setLimit(0, 3.14);
    }

    Door* d = new DoorClassic(doorObj, armObject, hingeBusToArm, hingeArmToDoor, openSoundComp, closeSoundComp, velocity, rotationDirection, group);
    d->close();
    _bus->_doors.push_back(d);
}


void BusLoader::loadEnvironmentCaptureComponents(XMLElement* moduleElement, BusRayCastModule& busModule)
{
    XMLElement* componentElement = moduleElement->FirstChildElement("Component");
    if (componentElement != nullptr)
    {
        std::string type = std::string(componentElement->Attribute("type"));
        if (type == "environmentCapture")
        {
            Logger::info("XML: environmentCapture component data");

            std::string textures = std::string(componentElement->Attribute("textures"));
            std::string t[6];
            istringstream stream(textures);
            std::string s;
            int index = 0;
            while (getline(stream, s, ',')) {
                t[index++] = _busPath + s;
            }
            RTextureCubeMap* cubeMap = ResourceManager::getInstance().loadTextureCubeMap(t);
            busModule.sceneObject->addComponent(GraphicsManager::getInstance().addEnvironmentCaptureComponent(cubeMap));
        }
    }
}


void BusLoader::loadMirrors(XMLElement* moduleElement, BusRayCastModule& busModule)
{
    XMLElement* mirrorElement = moduleElement->FirstChildElement("Mirror");
	if (!GameConfig::getInstance().isMirrorsEnabled)
	{
		return;
	}

    while (mirrorElement != nullptr)
    {
        Logger::info("XML: Mirror component data");

        std::string name(mirrorElement->Attribute("name"));
        glm::vec3 position = XMLstringToVec3(mirrorElement->Attribute("position"));
        glm::vec3 rotation = XMLstringToVec3(mirrorElement->Attribute("rotation"));
        glm::vec3 normal = XMLstringToVec3(mirrorElement->Attribute("normal"));
		float renderingDistance = GameConfig::getInstance().mirrorRenderingDistance;

        SceneObject* mirrorObject = _sMgr->addSceneObject(name);
        MirrorComponent* mirrorComponent = GraphicsManager::getInstance().addMirrorComponent(name, renderingDistance);
        mirrorComponent->setNormalVector(normal);

        mirrorObject->addComponent(mirrorComponent);
        mirrorObject->setPosition(position);
        mirrorObject->setRotation(rotation);
        busModule.sceneObject->addChild(mirrorObject);

        _bus->_mirrors.push_back(mirrorComponent);

        mirrorElement = mirrorElement->NextSiblingElement("Mirror");
    }
}


void BusLoader::loadDisplays(XMLElement* moduleElement, BusRayCastModule& busModule)
{
	XMLElement* displayElement = moduleElement->FirstChildElement("Display");
	while (displayElement != nullptr)
	{
		Logger::info("XML: Display component data");

		std::string name(displayElement->Attribute("name"));
		glm::vec3 position = XMLstringToVec3(displayElement->Attribute("position"));
		glm::vec3 rotation = XMLstringToVec3(displayElement->Attribute("rotation"));
		float width = (float)atof(displayElement->Attribute("width"));
		float height = (float)atof(displayElement->Attribute("height"));
		int widthInPoints = (int)atoi(displayElement->Attribute("widthInPoints"));;
		int heightInPoints = (int)atoi(displayElement->Attribute("heightInPoints"));;
		std::string fontName = std::string(displayElement->Attribute("font"));
		int type = XmlUtils::getAttributeIntOptional(displayElement, "type");
        glm::vec3 textColor = XmlUtils::getAttributeVec3Optional(displayElement, "textColor", DisplayComponent::DEFAULT_TEXT_COLOR);

		SceneObject* displaySceneObject = _sMgr->addSceneObject(name);
		displaySceneObject->setPosition(position);
		displaySceneObject->setRotation(rotation);
		busModule.sceneObject->addChild(displaySceneObject);

		Material* material = new Material;
		material->shininess = 96;
		material->shader = SOLID_EMISSIVE_MATERIAL;
		Prefab* displayRenderObject = new PlanePrefab(glm::vec2(width, height), material);
		displayRenderObject->init();
		displayRenderObject->setIsDynamicObject(true);
		GraphicsManager::getInstance().addRenderObject(displayRenderObject, displaySceneObject);

		RDisplayFont* displayFont = ResourceManager::getInstance().loadDisplayFont(fontName);
		DisplayComponent* displayComponent = GraphicsManager::getInstance().addDisplayComponent(displayFont, widthInPoints, heightInPoints, textColor);
		displaySceneObject->addComponent(displayComponent);
		displayComponent->init();

		_bus->_displays.push_back(std::make_pair(displayComponent, static_cast<BusDisplayType>(type)));

		displayElement = displayElement->NextSiblingElement("Display");
	}
}


void BusLoader::loadModulesConnectionData(XMLElement* moduleElement, BusRayCastModule& busModule)
{
    XMLElement* jointElement = moduleElement->FirstChildElement("Joint");
    while (jointElement != nullptr)
    {
        Logger::info("XML: Joint element data");

        busModule.jointPosition = XMLstringToBtVec3(jointElement->Attribute("position"));

        jointElement = jointElement->NextSiblingElement("Joint");
    }
}


SoundComponent* BusLoader::createSound(SceneObject* soundObject, const SoundDefinition& soundDefinition)
{
    RSound* engineSound = ResourceManager::getInstance().loadSound(soundDefinition.soundFilename);
    SoundComponent* soundComp = new SoundComponent(engineSound, EST_PLAYER, soundDefinition.looped);
    soundObject->addComponent(soundComp);
    soundComp->setGain(soundDefinition.volume);
    soundComp->setPlayDistance(soundDefinition.playDistance);

    _sndMgr->addSoundComponent(soundComp);

    return soundComp;
}


void BusLoader::createRequireSoundComponents()
{
    // Create announcement source component
    SoundComponent* announcementSource = new SoundComponent(NULL, EST_PLAYER, false);
    _bus->_modules[0].sceneObject->addComponent(announcementSource);
    announcementSource->setGain(1.0f);
    announcementSource->setPlayDistance(20.0f);

    _sndMgr->addSoundComponent(announcementSource);

    _bus->_announcementSource = announcementSource;
}
