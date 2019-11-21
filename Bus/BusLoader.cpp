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
    _busCollidesWith(COL_TERRAIN | COL_ENV), _wheelCollidesWith(COL_TERRAIN | COL_ENV), _doorCollidesWith(COL_TERRAIN | COL_ENV)
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


    loadEngineAndGearbox(doc);


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
    _bus->_engine = std::move(_engine);
    _bus->_gearbox = std::move(_gearbox);

    bool result = loadBusModules(busElement);
    if (!result)
    {
        delete _bus;
        return NULL;
    }

    return _bus;
}


void BusLoader::loadEngineAndGearbox(XMLDocument& doc)
{
    XMLElement* objElement = doc.FirstChildElement("Gearbox");
    std::string gearboxFile(objElement->Attribute("model"));
    _gearbox = std::unique_ptr<Gearbox> (new Gearbox(gearboxFile));

    objElement = doc.FirstChildElement("Engine");
    std::string engineFile(objElement->Attribute("model"));
    _engine = std::unique_ptr<Engine> (new Engine(engineFile));
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
        loadModelNodes(moduleElement, modelPath, _texturePath, nodeToSkip, _cachedModelNodes);

        RStaticModel* busModel;
        if (nodeToSkip.empty())
            busModel = ResourceManager::getInstance().loadModel(modelPath, _texturePath, _normalsSmoothing);
        else
            busModel = ResourceManager::getInstance().loadModelWithHierarchy(modelPath, _texturePath, nodeToSkip, _normalsSmoothing);

        RenderObject* busRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel), busModule.sceneObject);
		busRenderObject->setIsDynamicObject(true);


        // Tworzenie fizycznego obiektu karoserii

        if (busModel->getCollisionMeshSize() > 0)
        {
            int wheelCollidesWith = COL_TERRAIN | COL_ENV;

            busModule.rayCastVehicle = _pMgr->createPhysicalBodyRayCastVehicle(busModel->getCollisionMesh(), busModel->getCollisionMeshSize(), mass, COL_BUS, _busCollidesWith);
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
        _bus->_desktop = new Desktop(NULL);
    }

	_bus->updateDisplays();

    return true;
}


void BusLoader::loadModelNodes(XMLElement* moduleElement, std::string modelPath, std::string texturePath, std::vector<std::string>& modelNodesNames,
                               std::unordered_map<std::string, ModelNodeAndTransform>& modelNodes)
{
    XMLElement* doorElement = moduleElement->FirstChildElement("Door");
    while (doorElement != nullptr)
    {
        std::string doorModelNodeName = XmlUtils::getAttributeStringOptional(doorElement, "modelNode");
        std::string armModelNodeName = XmlUtils::getAttributeStringOptional(doorElement, "armModelNode");

        if (!doorModelNodeName.empty())
            modelNodesNames.push_back(doorModelNodeName);

        if (!armModelNodeName.empty())
            modelNodesNames.push_back(armModelNodeName);

        doorElement = doorElement->NextSiblingElement("Door");
    }

    if (!modelNodesNames.empty())
    {
        std::vector<Transform> loadedNodesTransformsInModel;
        std::vector<RStaticModel*> loadedNodes;

        ResourceManager::getInstance().loadModelWithHierarchyOnlyNodes(modelPath, texturePath, modelNodesNames, loadedNodesTransformsInModel, loadedNodes, _normalsSmoothing);

        for (int i = 0; i < modelNodesNames.size(); ++i)
        {
            modelNodes[modelNodesNames[i]] = ModelNodeAndTransform(loadedNodes[i], loadedNodesTransformsInModel[i]);
        }
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
        steeringWheelObject->setRotation(glm::vec3(rotation.x * PI, rotation.y * PI, rotation.z * PI));
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
        _bus->_desktop = new Desktop(_bus->_desktopRenderObject);

        XMLElement* indicatorElement = desktopElement->FirstChildElement("Indicator");
        while (indicatorElement != nullptr)
        {
            const char* cType = indicatorElement->Attribute("type");
            DesktopIndicatorType type = getDesktopIndicatorTypeFromString(cType);

            std::string modelNodeName(indicatorElement->Attribute("modelNodeName"));
            float maxAngle = (float)atof(indicatorElement->Attribute("maxAngle"));
            float maxValue = (float)atof(indicatorElement->Attribute("maxValue"));
            float minValue = XmlUtils::getAttributeFloatOptional(indicatorElement, "minValue");

            _bus->_desktop->setIndicator(type, modelNodeName, degToRad(maxAngle), maxValue, minValue);

            indicatorElement = indicatorElement->NextSiblingElement("Indicator");
        }

        XMLElement* buttonElement = desktopElement->FirstChildElement("Button");
        while (buttonElement != nullptr)
        {
            const char* cType = buttonElement->Attribute("type");
            DesktopButtonType type = getDesktopButtonTypeFromString(cType);

            std::string modelNodeName(buttonElement->Attribute("modelNodeName"));
            bool isReturning = XmlUtils::getAttributeBool(buttonElement, "isReturning");

            std::vector<glm::vec3> translationForStates;
            std::vector<glm::vec3> rotationForStates;

            XMLElement* buttonStateElement = buttonElement->FirstChildElement("State");
            while (buttonStateElement != nullptr)
            {
                glm::vec3 translation = XMLstringToVec3(buttonStateElement->Attribute("translation"));
                translationForStates.push_back(translation);

                glm::vec3 rotation = XMLstringToVec3(buttonStateElement->Attribute("rotation"));
                rotationForStates.push_back(rotation);

                buttonStateElement = buttonStateElement->NextSiblingElement("State");
            }

            _bus->_desktop->setButton(type, modelNodeName, translationForStates, rotationForStates, isReturning);

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
        if (doorModelName.empty())
        {
            isDoorLoadedFromSeparateModel = false;
            doorModelNodeName = XmlUtils::getAttributeString(doorElement, "modelNode");
        }


        glm::vec3 doorPosition;
        glm::vec3 doorRotation;
        RStaticModel* doorModel;
        if (isDoorLoadedFromSeparateModel)
        {
            std::string doorModelPath = _busPath + doorModelName;

            doorModel = ResourceManager::getInstance().loadModel(doorModelPath, _texturePath, _normalsSmoothing);

            doorPosition = XmlUtils::getAttributeVec3(doorElement, "position");
            doorRotation = XmlUtils::getAttributeVec3Optional(doorElement, "rotation");
        }
        else
        {
            doorModel = _cachedModelNodes[doorModelNodeName].model;

            Transform& doorTransform = _cachedModelNodes[doorModelNodeName].transform;
            doorPosition = doorTransform.getPosition();
            doorRotation = doorTransform.getRotation();
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
            loadDoorClassic(doorElement, busModule, doorObj, doorModel, mass, group, openSoundComp, closeSoundComp, isDoorLoadedFromSeparateModel);
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


void BusLoader::loadDoorClassic(XMLElement* doorElement, BusRayCastModule& busModule, SceneObject* doorObj, RStaticModel* doorModel, float mass, char group,
                                SoundComponent* openSoundComp, SoundComponent* closeSoundComp, bool isDoorLoadedFromSeparateModel)
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
    if (armModelName.empty())
    {
        isArmLoadedFromSeparateMode = false;
        armModelNodeName = XmlUtils::getAttributeString(doorElement, "armModelNode");
    }


    RStaticModel* armModel;
    glm::vec3 armPosition;
    glm::vec3 armRotation;

    if (isArmLoadedFromSeparateMode)
    {
        std::string armModelPath = _busPath + armModelName;

        armModel = ResourceManager::getInstance().loadModel(armModelPath, _texturePath, _normalsSmoothing);

        armPosition = XmlUtils::getAttributeVec3(doorElement, "armPosition");
        armRotation = XmlUtils::getAttributeVec3(doorElement, "armRotation");
    }
    else
    {
        armModel = _cachedModelNodes[armModelNodeName].model;

        Transform& armTransform = _cachedModelNodes[armModelNodeName].transform;
        armPosition = armTransform.getPosition();
        armRotation = armTransform.getRotation();
    }


    SceneObject* armObject = _sMgr->addSceneObject("arm");
    armObject->setPosition(armPosition);
    armObject->setRotation(armRotation);

    RenderObject* armRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(armModel), armObject);
	armRenderObject->setIsDynamicObject(true);

    PhysicalBodyConvexHull* armBody = _pMgr->createPhysicalBodyConvexHull(armModel->getCollisionMesh(), armModel->getCollisionMeshSize(), armMass, COL_DOOR, COL_NOTHING);
    armObject->addComponent(armBody);


    // door object
    RenderObject* doorRenderObject = GraphicsManager::getInstance().addRenderObject(new RenderObject(doorModel), doorObj);
	doorRenderObject->setIsDynamicObject(true);

    PhysicalBodyConvexHull* doorBody = _pMgr->createPhysicalBodyConvexHull(doorModel->getCollisionMesh(), doorModel->getCollisionMeshSize(), mass, COL_DOOR, _doorCollidesWith);
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

		SceneObject* displaySceneObject = _sMgr->addSceneObject(name);
		displaySceneObject->setPosition(position);
		displaySceneObject->setRotation(rotation);
		busModule.sceneObject->addChild(displaySceneObject);

		Material material;
		material.shininess = 96;
		material.shader = SOLID_EMISSIVE_MATERIAL;
		Prefab* displayRenderObject = new PlanePrefab(glm::vec2(width, height), material);
		displayRenderObject->init();
		displayRenderObject->setIsDynamicObject(true);
		GraphicsManager::getInstance().addRenderObject(displayRenderObject, displaySceneObject);

		RDisplayFont* displayFont = ResourceManager::getInstance().loadDisplayFont(fontName);
		DisplayComponent* displayComponent = GraphicsManager::getInstance().addDisplayComponent(displayFont, widthInPoints, heightInPoints);
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


void BusLoader::createRequireSoundComponents()
{
    // Create Sound Component if sound filename is defined in Engine XML config file
    if (_bus->_engine->getSoundFilename() != "")
    {
        RSound* engineSound = ResourceManager::getInstance().loadSound(_bus->_engine->getSoundFilename());
        SoundComponent* soundComp = new SoundComponent(engineSound, EST_PLAYER, true);
        //_sceneObject->addComponent(soundComp);
        _bus->_modules[0].sceneObject->addComponent(soundComp);
        soundComp->setGain(_bus->_engine->getSoundVolume());
        soundComp->setPlayDistance(20.0f);

        _sndMgr->addSoundComponent(soundComp);
    }

    // Create announcement source component
    SoundComponent* announcementSource = new SoundComponent(NULL, EST_PLAYER, false);
    _bus->_modules[0].sceneObject->addComponent(announcementSource);
    announcementSource->setGain(1.0f);
    announcementSource->setPlayDistance(20.0f);

    _sndMgr->addSoundComponent(announcementSource);

    _bus->_announcementSource = announcementSource;
}
