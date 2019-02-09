#include "BusConstraint.h"

// XML reader
#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

#include "../Utils/Math.h"

#include "../Graphics/GraphicsManager.h"
#include "../Utils/ResourceManager.h"

//#include "SoundComponent.h"

#include "../Utils/Helpers.hpp"

BusConstraint::BusConstraint(SceneManager* smgr, PhysicsManager* pmgr, SoundManager* sndMgr, std::string filename)
: _sceneObject(0), _chasisBody(0), _sMgr(smgr), _pMgr(pmgr), _sndMgr(sndMgr),
_maxSteerAngle(0.55f), _steerStep(0.3f),
_brake(false), _accelerate(false), _handbrake(true), _idle(true),
_brakeForce(0.0f), _brakeForceStep(0.1f),
_steeringWheelObject(NULL), _driverPosition(0.0f, 0.0f, 0.0f),
_isEnableLights(false), _isEnableHeadlights(false),
_collidesWith(COL_TERRAIN | COL_ENV)
{
    std::cout << "Bus Konstruktor" << std::endl;

    loadXMLdata(filename);
}

BusConstraint::~BusConstraint()
{
    std::cout << "Bus Destruktor" << std::endl;

    WheelList::iterator wit = _wheels.begin();

    for (; wit != _wheels.end(); ++wit)
    {
        _pMgr->removeConstraint((*wit)->suspension);

        delete (*wit);
    }
    _wheels.clear();

    DoorList::iterator dit = _doors.begin();

    for (; dit != _doors.end(); ++dit)
    {

        delete (*dit);
    }
    _doors.clear();

    _headlights.clear();
}


void BusConstraint::loadXMLdata(std::string busname)
{
    std::string filename = "Buses/" + busname + "/config.xml";

    XMLDocument doc;
    doc.LoadFile( filename.c_str() );

    XMLElement* objElement = doc.FirstChildElement("Gearbox");

    std::string gearboxFile(objElement->Attribute("model"));
    _gearbox = std::unique_ptr<Gearbox> (new Gearbox(gearboxFile));

    objElement = doc.FirstChildElement("Engine");
    std::string engineFile(objElement->Attribute("model"));
    _engine = std::unique_ptr<Engine> (new Engine(engineFile));

    XMLElement* busElement = doc.FirstChildElement("Bus");

    std::cout << "XML DATA" << std::endl;

    std::string sObjName(busElement->Attribute("name"));


    std::string sTextureFolder(busElement->Attribute("textures"));

    std::cout<< sObjName << std::endl;

    RStaticModel* busModel = 0;

    glm::vec3 busPosition = glm::vec3(0,0,0);
    //glm::vec3 busRotation = glm::vec3(0,0,0);

    std::string texturePath = "Buses/" + busname + "/" + sTextureFolder + "/";


    Module busModule;



    // Reading bus body configuration
    XMLElement* moduleElement = busElement->FirstChildElement("Module");

    while (moduleElement != nullptr)
    {

        //_sceneObject = _sMgr->addSceneObject(sObjName);
        busModule.sceneObject = _sMgr->addSceneObject(sObjName);

        std::string modelFile = std::string(moduleElement->Attribute("model"));
        std::string modelPath = "Buses/" + busname + "/" + modelFile;

        glm::vec3 modulePosition = glm::vec3(0,0,0);
        modulePosition = XMLstringToVec3(moduleElement->Attribute("position"));

        busModule.sceneObject->setPosition(modulePosition);

        busModel = ResourceManager::getInstance().loadModel(modelPath, texturePath);
        //GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel), _sceneObject);
        GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel), busModule.sceneObject);

        //_sceneObject->addComponent(renderObj);

        // Tworzenie fizycznego obiektu karoserii
        const char* cMass = moduleElement->Attribute("mass");
        float fMass = (float)atof(cMass);

        if (busModel->getCollisionMeshSize() > 0)
        {
            //_chasisBody = _pMgr->createPhysicalBodyConvexHull(busModel->getCollisionMesh(), busModel->getCollisionMeshSize(), fMass, COL_BUS, _collidesWith);
            //_chasisBody->getRigidBody()->setActivationState( DISABLE_DEACTIVATION );

            busModule.body = _pMgr->createPhysicalBodyConvexHull(busModel->getCollisionMesh(), busModel->getCollisionMeshSize(), fMass, COL_BUS, _collidesWith);
            busModule.body->getRigidBody()->setActivationState( DISABLE_DEACTIVATION );

            //_sceneObject->addComponent(_chasisBody);
            busModule.sceneObject->addComponent(busModule.body);

            //btVector3 dirVec(0,0,1);
            //btTransform tmpTransf = _chasisBody->getRigidBody()->getWorldTransform();
            //btVector3 dir = tmpTransf.getBasis() * dirVec;
        }
        else
        {
            std::cout << "ERROR: Collision mesh not found in bus model!\n" << std::endl;
            return;
        }

        // ########### WHEELS ###########
        XMLElement* wheelElement = moduleElement->FirstChildElement("Wheel");
        while (wheelElement != nullptr)
        {
            std::cout << "XML: Wheel data" << std::endl;

            std::string wheelName(wheelElement->Attribute("name"));
            std::cout << wheelName << std::endl;
            std::string wheelModel(wheelElement->Attribute("model"));
            std::string side(wheelElement->Attribute("side"));
            float mass = (float)atof(wheelElement->Attribute("mass"));
            float radius = (float)atof(wheelElement->Attribute("radius"));
            float width = (float)atof(wheelElement->Attribute("width"));

            WheelSide wheelSide;

            int steering = (int)atoi(wheelElement->Attribute("steering"));
            int powered = (int)atoi(wheelElement->Attribute("powered"));

            float stiffness = (float)atof(wheelElement->Attribute("stiffness"));
            float damping = (float)atof(wheelElement->Attribute("damping"));
            float brakeForce = (float)atof(wheelElement->Attribute("brakeForce"));

            SceneObject* wheelObj = _sMgr->addSceneObject(wheelName);


            glm::vec3 wheelPosition = XMLstringToVec3(wheelElement->Attribute("position"));
            //glm::vec3 relativePos = _chasisBody->getSceneObject()->transformLocalPointToGlobal(wheelPosition);

            wheelObj->setPosition(wheelPosition);

            // obracamy model kola je¿li jest po lewej stronie
            if (side == "right")
            {
                wheelSide = WS_RIGHT;
            }
            else
                wheelSide = WS_LEFT;

            btVector3 btWheelPos(wheelPosition.x, wheelPosition.y, wheelPosition.z);

            std::string modelPath = "Buses/" + busname + "/" + wheelModel;
            RStaticModel* wheel = ResourceManager::getInstance().loadModel(modelPath, texturePath);
            GraphicsManager::getInstance().addRenderObject(new RenderObject(wheel), wheelObj);

            //wheelObj->addComponent(wheelRender);

            int collidesWith = COL_TERRAIN | COL_ENV;
            PhysicalBodyCylinder* wheelCyl = _pMgr->createPhysicalBodyCylinder(btVector3(width, radius, radius), mass, X_AXIS, COL_WHEEL, collidesWith);
            wheelCyl->getRigidBody()->setFriction(10.0f);
            wheelCyl->getRigidBody()->setRestitution(0.1f);
            //wheelCyl->getRigidBody()->setFriction(1.0f);
            wheelObj->addComponent(wheelCyl);

            //ConstraintHinge2* hinge1 = _pMgr->createConstraintHinge2(_chasisBody, wheelCyl, btWheelPos, btVector3(0,1,0), btVector3(1,0,0));
            ConstraintHinge2* hinge1 = _pMgr->createConstraintHinge2(busModule.body, wheelCyl, btWheelPos, btVector3(0,1,0), btVector3(1,0,0));
            hinge1->setStiffness(2, stiffness);
            hinge1->setDamping(2, damping);
            hinge1->getBulletConstraint()->setLinearUpperLimit(btVector3(0,0,0.25));
            hinge1->getBulletConstraint()->setLinearLowerLimit(btVector3(0,0,-0.14));


            hinge1->getBulletConstraint()->setUpperLimit(0.0f);
            hinge1->getBulletConstraint()->setLowerLimit(0.0f);


            btVector3 lowerlimit;
            hinge1->getBulletConstraint()->getLinearLowerLimit(lowerlimit);
            //std::cout << lowerlimit.x() << " " << lowerlimit.y() << " " << lowerlimit.z() << std::endl;

            btVector3 upperlimit;
            hinge1->getBulletConstraint()->getLinearUpperLimit(upperlimit);
            //std::cout << upperlimit.x() << " " << upperlimit.y() << " " << upperlimit.z() << std::endl;

            Wheel* w = new Wheel;
            w->body = wheelCyl;
            w->suspension = hinge1;
            w->currentAngle = 0.0f;
            w->brakeForce = brakeForce;
            w->steering = steering;
            w->powered = powered;
            w->wheelSide = wheelSide;

            _wheels.push_back(w);

            wheelElement = wheelElement->NextSiblingElement("Wheel");
        }

        // ########### INTERIOR LIGHTS ###########
        XMLElement* lightElement = moduleElement->FirstChildElement("Light");
        while (lightElement != nullptr)
        {
            //std::cout << "XML: Light" << std::endl;

            const char* cPosition = lightElement->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);

            const char* cColor = lightElement->Attribute("color");
            glm::vec3 color = XMLstringToVec3(cColor);

            const char* cAttenuation = lightElement->Attribute("attenuation");
            glm::vec3 attenuation = XMLstringToVec3(cAttenuation);

            float ambientIntensity = (float)atof(lightElement->Attribute("ambientIntensity"));
            float diffuseIntensity = (float)atof(lightElement->Attribute("diffuseIntensity"));


            SceneObject* light = _sMgr->addSceneObject("busLight" + toString(_lights.size()));
            Light* lightComponent = GraphicsManager::getInstance().addPointLight(color, ambientIntensity,
                                                                                 diffuseIntensity,
                                                                                 LightAttenuation(attenuation.x, attenuation.y, attenuation.z));
            light->addComponent(lightComponent);
            light->setPosition(position);
            //_sceneObject->addChild(light);
            busModule.sceneObject->addChild(light);

            lightComponent->setIsActive(_isEnableLights);

            _lights.push_back(lightComponent);

            lightElement = lightElement->NextSiblingElement("Light");
        }

        // ########### DRIVER'S POSITION ###########
        XMLElement* driverPositionElement = busElement->FirstChildElement("Driver");
        if (driverPositionElement != nullptr)
        {
            const char* cPosition = driverPositionElement->Attribute("position");
            _driverPosition = XMLstringToVec3(cPosition);

            driverPositionElement = driverPositionElement->NextSiblingElement("Driver");
        }





        // ########### STEERING WHEEL ###########
        XMLElement* steeringWheelElement = moduleElement->FirstChildElement("SteeringWheel");
        if (steeringWheelElement != nullptr)
        {
            //std::cout << "XML: Steering wheel" << std::endl;

            std::string modelFile = std::string(steeringWheelElement->Attribute("model"));
            std::string modelPath = "Buses/" + busname + "/" + modelFile;

            _steeringWheelObject = _sMgr->addSceneObject("steeringWheel");

            RStaticModel* steeringWheelModel = ResourceManager::getInstance().loadModel(modelPath, "./");
            GraphicsManager::getInstance().addRenderObject(new RenderObject(steeringWheelModel), _steeringWheelObject);

            const char* cPosition = steeringWheelElement->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);
            _steeringWheelObject->setPosition(position);

            const char* cRotation = steeringWheelElement->Attribute("rotation");
            glm::vec3 rotation(XMLstringToVec3(cRotation));
            _steeringWheelObject->setRotation(glm::vec3(rotation.x * PI, rotation.y * PI, rotation.z * PI) );

            const char* cScale = steeringWheelElement->Attribute("scale");
            glm::vec3 scale(XMLstringToVec3(cScale));
            _steeringWheelObject->setScale(scale);

            //_sceneObject->addChild(_steeringWheelObject);
            busModule.sceneObject->addChild(_steeringWheelObject);
        }

        // ########### HEADLIGHTS ###########
        XMLElement* headlightElement = moduleElement->FirstChildElement("Headlight");
        while (headlightElement != nullptr)
        {
            //std::cout << "XML: Headlights" << std::endl;

            std::string headlightName(headlightElement->Attribute("name"));
            const char* cPosition = headlightElement->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);

            const char* cRotation = headlightElement->Attribute("rotation");
            glm::vec3 rotation = XMLstringToVec3(cRotation);

            const char* cColor = headlightElement->Attribute("color");
            glm::vec3 color = XMLstringToVec3(cColor);

            const char* cAttenuation = headlightElement->Attribute("attenuation");
            glm::vec3 attenuation = XMLstringToVec3(cAttenuation);

            float ambientIntensity = (float)atof(headlightElement->Attribute("ambientIntensity"));
            float diffuseIntensity = (float)atof(headlightElement->Attribute("diffuseIntensity"));
            float cutoff = (float)atof(headlightElement->Attribute("cutoff"));


            SceneObject* light = _sMgr->addSceneObject(headlightName);
            Light* lightComponent = GraphicsManager::getInstance().addSpotLight(color, ambientIntensity,
                                                                                diffuseIntensity, cutoff,
                                                                                LightAttenuation(attenuation.x, attenuation.y, attenuation.z));
            light->addComponent(lightComponent);
            light->setPosition(position);
            light->setRotation(rotation);
            //_sceneObject->addChild(light);
            busModule.sceneObject->addChild(light);

            lightComponent->setIsActive(_isEnableHeadlights);

            _headlights.push_back(lightComponent);

            headlightElement = headlightElement->NextSiblingElement("Headlight");
        }

        // ########### DOORS ###########
        XMLElement* doorElement = moduleElement->FirstChildElement("Door");
        while (doorElement != nullptr)
        {
            std::cout << "XML: Door data" << std::endl;

            std::string doorName(doorElement->Attribute("name"));
            std::string doorType(doorElement->Attribute("type"));
            std::string doorModel(doorElement->Attribute("model"));
            float mass = (float)atof(doorElement->Attribute("mass"));
            char group = (char)atoi(doorElement->Attribute("group"));

            std::string openSound = "Buses/" + busname + "/" + std::string(doorElement->Attribute("doorOpenSound"));
            std::string closeSound = "Buses/" + busname + "/" + std::string(doorElement->Attribute("doorCloseSound"));

            std::cout << "Door open sound:" << openSound << std::endl;
            std::cout << "Door open sound:" << closeSound <<std::endl;


            // Create sound component
            RSound* openSoundResource = ResourceManager::getInstance().loadSound(openSound);
            SoundComponent* openSoundComp = new SoundComponent(openSoundResource, EST_PLAYER);
            _sndMgr->addSoundComponent(openSoundComp);

            RSound* closeSoundResource = ResourceManager::getInstance().loadSound(closeSound);
            SoundComponent* closeSoundComp = new SoundComponent(closeSoundResource, EST_PLAYER);
            _sndMgr->addSoundComponent(closeSoundComp);

            SceneObject* doorObj = 0;

            if (doorType == "s")
            {

                glm::vec3 doorPosition = XMLstringToVec3(doorElement->Attribute("position"));
                glm::vec3 relativePos = busModule.sceneObject->transformLocalPointToGlobal(doorPosition);

                // poczatek IF
                btVector3 busPivot = XMLstringToBtVec3(doorElement->Attribute("pivotA"));
                btVector3 doorPivot = XMLstringToBtVec3(doorElement->Attribute("pivotB"));

                doorObj = _sMgr->addSceneObject(doorName);
                doorObj->setPosition(doorPosition);

                std::string modelPath = "Buses/" + busname + "/" + doorModel;

                RStaticModel* dr = ResourceManager::getInstance().loadModel(modelPath, texturePath);
                GraphicsManager::getInstance().addRenderObject(new RenderObject(dr), doorObj);

                btVector3 btDoorPos(relativePos.x, relativePos.y, relativePos.z);

                int collidesWith = COL_ENV | COL_TERRAIN;
                PhysicalBodyConvexHull* doorBody = _pMgr->createPhysicalBodyConvexHull(dr->getCollisionMesh(), dr->getCollisionMeshSize(), mass, COL_DOOR, collidesWith);
                doorObj->addComponent(doorBody);

                //ConstraintHinge* doorHinge = _pMgr->createConstraintHinge(_chasisBody, doorBody, busPivot, doorPivot, btVector3(0,1,0), btVector3(0,1,0));
                ConstraintHinge* doorHinge = _pMgr->createConstraintHinge(busModule.body, doorBody, busPivot, doorPivot, btVector3(0,1,0), btVector3(0,1,0));

                doorHinge->getBulletConstraint()->setLimit(-1.5,0);

                Door* d = 0;
                d = new DoorSimple(dr, doorBody, doorHinge, openSoundComp, closeSoundComp, group);
                _doors.push_back(d);
            } // IF "S"
            else
            if (doorType == "se")
            {
                std::string armName(doorElement->Attribute("armName"));
                std::string armModel(doorElement->Attribute("arm"));
                float armMass = (float)atof(doorElement->Attribute("armMass"));
                char group = (char)atoi(doorElement->Attribute("group"));

                //float arm1lowLimit = (float)atof(doorElement->Attribute("arm1lowLimit"));
                //float arm1highLimit = (float)atof(doorElement->Attribute("arm1highLimit"));
                glm::vec2 arm1limits = XMLstringToVec2(doorElement->Attribute("arm1limits"));

                std::string rotDir(doorElement->Attribute("rotationDir"));

                RotationDir rdir;

                if (rotDir == "CCW")
                    rdir = ERD_CCW;
                else
                    rdir = ERD_CW;

                glm::vec3 armPosition = XMLstringToVec3(doorElement->Attribute("armPosition"));
                glm::vec3 armRelPos = glm::vec3(busPosition.x + armPosition.x, busPosition.y + armPosition.y, busPosition.z + armPosition.z);

                btVector3 armPivotA = XMLstringToBtVec3(doorElement->Attribute("armPivotA"));
                btVector3 armPivotB = XMLstringToBtVec3(doorElement->Attribute("armPivotB"));

                SceneObject* armObj = _sMgr->addSceneObject(armName);
                armObj->setPosition(armRelPos);

                std::string armPath = "Buses/" + busname + "/" + armModel;

                RStaticModel* arm = ResourceManager::getInstance().loadModel(armPath, texturePath);
                GraphicsManager::getInstance().addRenderObject(new RenderObject(arm), armObj);

                btVector3 btArmPos(armRelPos.x, armRelPos.y, armRelPos.z);

                int collidesWith = COL_ENV | COL_TERRAIN;
                PhysicalBodyConvexHull* armBody = _pMgr->createPhysicalBodyConvexHull(arm->getCollisionMesh(), arm->getCollisionMeshSize(), armMass, COL_DOOR, collidesWith);
                armObj->addComponent(armBody);

                //ConstraintHinge* busArmHinge = _pMgr->createConstraintHinge(_chasisBody, armBody, armPivotA, armPivotB, btVector3(0,1,0), btVector3(0,1,0));
                ConstraintHinge* busArmHinge = _pMgr->createConstraintHinge(busModule.body, armBody, armPivotA, armPivotB, btVector3(0,1,0), btVector3(0,1,0));

                busArmHinge->getBulletConstraint()->setLimit(arm1limits.x,arm1limits.y);

                // arm 2

                std::string arm2Name(doorElement->Attribute("arm2Name"));
                std::string arm2Model(doorElement->Attribute("arm2"));
                float arm2Mass = (float)atof(doorElement->Attribute("arm2Mass"));

                //float arm2lowLimit = (float)atof(doorElement->Attribute("arm2lowLimit"));
                //float arm2highLimit = (float)atof(doorElement->Attribute("arm2highLimit"));

                glm::vec3 arm2Position = XMLstringToVec3(doorElement->Attribute("arm2Position"));
                glm::vec3 arm2RelPos = glm::vec3(busPosition.x + arm2Position.x, busPosition.y + arm2Position.y, busPosition.z + arm2Position.z);

                btVector3 arm2PivotA = XMLstringToBtVec3(doorElement->Attribute("arm2PivotA"));
                btVector3 arm2PivotB = XMLstringToBtVec3(doorElement->Attribute("arm2PivotB"));

                SceneObject* arm2Obj = _sMgr->addSceneObject(arm2Name);
                arm2Obj->setPosition(arm2RelPos);

                std::string arm2Path = "Buses/" + busname + "/" + arm2Model;

                RStaticModel* arm2 = ResourceManager::getInstance().loadModel(arm2Path, texturePath);
                GraphicsManager::getInstance().addRenderObject(new RenderObject(arm2), arm2Obj);
                //arm2Obj->addComponent(arm2Render);

                btVector3 btArm2Pos(arm2RelPos.x, arm2RelPos.y, arm2RelPos.z);

                collidesWith = COL_NOTHING;
                PhysicalBodyConvexHull* arm2Body = _pMgr->createPhysicalBodyConvexHull(arm2->getCollisionMesh(), arm2->getCollisionMeshSize(), arm2Mass, COL_DOOR, collidesWith);
                arm2Obj->addComponent(arm2Body);

                //_pMgr->createConstraintHinge(_chasisBody, arm2Body, arm2PivotA, arm2PivotB, btVector3(0,1,0), btVector3(0,1,0));
                _pMgr->createConstraintHinge(busModule.body, arm2Body, arm2PivotA, arm2PivotB, btVector3(0,1,0), btVector3(0,1,0));

                //busArm2Hinge->getBulletConstraint()->setLimit(arm2lowLimit,arm2highLimit);

                // door model

                std::string doorName(doorElement->Attribute("name"));
                std::string doorModel(doorElement->Attribute("model"));
                float doorMass = (float)atof(doorElement->Attribute("mass"));

                glm::vec3 doorPosition = XMLstringToVec3(doorElement->Attribute("position"));
                glm::vec3 relativePos = glm::vec3(armPosition.x + doorPosition.x, armPosition.y + doorPosition.y, armPosition.z + doorPosition.z);

                btVector3 doorPivotA = XMLstringToBtVec3(doorElement->Attribute("pivotA"));
                btVector3 doorPivotB = XMLstringToBtVec3(doorElement->Attribute("pivotB"));

                doorObj = _sMgr->addSceneObject(doorName);
                doorObj->setPosition(relativePos);

                std::string doorPath = "Buses/" + busname + "/" + doorModel;

                RStaticModel* door = ResourceManager::getInstance().loadModel(doorPath, texturePath);
                GraphicsManager::getInstance().addRenderObject(new RenderObject(door), doorObj);
                //doorObj->addComponent(doorRender);

                btVector3 btDoorPos(relativePos.x, relativePos.y, relativePos.z);

                collidesWith = COL_ENV;
                PhysicalBodyConvexHull* doorBody = _pMgr->createPhysicalBodyConvexHull(door->getCollisionMesh(), door->getCollisionMeshSize(), doorMass, COL_DOOR, collidesWith);
                doorObj->addComponent(doorBody);

                ConstraintHinge* armDoorHinge = _pMgr->createConstraintHinge(armBody, doorBody, doorPivotA, doorPivotB, btVector3(0,1,0), btVector3(0,1,0));

                if (rotDir == "CCW")
                    armDoorHinge->getBulletConstraint()->setLimit(-1.9,0.0);
                else
                    armDoorHinge->getBulletConstraint()->setLimit(0.0,1.9);

                btVector3 pivotC = XMLstringToBtVec3(doorElement->Attribute("pivotC"));
                btVector3 pivotD = XMLstringToBtVec3(doorElement->Attribute("pivotD"));

                _pMgr->createConstraintHinge(arm2Body, doorBody, pivotC, pivotD, btVector3(0,1,0), btVector3(0,1,0));


                Door* d = 0;
                d = new DoorSE(0, 0, arm, armBody, busArmHinge, 0, openSoundComp, closeSoundComp, rdir, group);
                _doors.push_back(d);
            }


            doorObj->addComponent(openSoundComp);
            doorObj->addComponent(closeSoundComp);

            doorElement = doorElement->NextSiblingElement("Door");
        }


        // Modules connection point
        XMLElement* jointElement = moduleElement->FirstChildElement("Joint");
        while (jointElement != nullptr)
        {
            busModule.jointPosition = XMLstringToBtVec3(jointElement->Attribute("position"));

            jointElement = jointElement->NextSiblingElement("Joint");
        }

        _modules.push_back(busModule);

        moduleElement = moduleElement->NextSiblingElement("Module");


    }

    // Create Sound Component if sound filename is defined in Engine XML config file
    if (_engine->getSoundFilename() != "")
    {
        RSound* engineSound = ResourceManager::getInstance().loadSound(_engine->getSoundFilename());
        SoundComponent* soundComp = new SoundComponent(engineSound, EST_PLAYER, true);
        //_sceneObject->addComponent(soundComp);
        _modules[0].sceneObject->addComponent(soundComp);
        soundComp->setGain(_engine->getSoundVolume());

        _sndMgr->addSoundComponent(soundComp);
    }

    // Create announcement source component
    _announcementSource = new SoundComponent(NULL, EST_PLAYER, false);
    _modules[0].sceneObject->addComponent(_announcementSource);
    _announcementSource->setGain(1.0f);
    _announcementSource->setPlayDistance(20.0f);

    _sndMgr->addSoundComponent(_announcementSource);

    // If it's articulated bus - connect modules with a ball joint
    if (_modules.size() > 1)
    {
        _pMgr->createConstraintBall(_modules[0].body, _modules[1].body, _modules[0].jointPosition, _modules[1].jointPosition);
    }

}

glm::vec3 BusConstraint::getDriverPosition()
{
    return _driverPosition;
}


void BusConstraint::setIsEnableLights(bool is)
{
    if (_lights.size() > 0)
    {
        for (LightsList::iterator i = _lights.begin(); i != _lights.end(); ++i)
        {
            (*i)->setIsActive(is);
        }
        _isEnableLights = is;
    }
}


bool BusConstraint::isEnableLights()
{
    return _isEnableLights;
}


void BusConstraint::setIsEnableHeadlights(bool is)
{
    if (_headlights.size() > 0)
    {
        for (HeadlightsList::iterator i = _headlights.begin(); i != _headlights.end(); ++i)
        {
            (*i)->setIsActive(is);
        }

        _isEnableHeadlights = is;
    }
}


bool BusConstraint::isEnableHeadlights()
{
    return _isEnableHeadlights;
}


void BusConstraint::turnLeft(float dt)
{
    WheelList::iterator it = _wheels.begin();

    for (; it != _wheels.end(); ++it)
    {
        Wheel* w = (*it);

        if ( w->steering )
        {
            if (w->currentAngle > -_maxSteerAngle)
            {
                w->currentAngle -= dt * _steerStep;

                if (_steeringWheelObject)
                {
                    /*glm::vec3 rotation = _steeringWheelObject->getTransform()->getRotation();
                    rotation.y += 0.005f;
                    _steeringWheelObject->getTransform()->setRotation(rotation);*/

                    _steeringWheelObject->rotate(0.0f, dt * _steerStep * 2 * PI / _maxSteerAngle, 0.0f);
                }
            }

            w->suspension->getBulletConstraint()->setLowerLimit(w->currentAngle);
            w->suspension->getBulletConstraint()->setUpperLimit(w->currentAngle);
        }
    }
}


void BusConstraint::turnRight(float dt)
{
    WheelList::iterator it = _wheels.begin();

    for (; it != _wheels.end(); ++it)
    {
        Wheel* w = (*it);

        if ( w->steering )
        {
            if (w->currentAngle < _maxSteerAngle)
            {
                w->currentAngle += dt * _steerStep;

                if (_steeringWheelObject)
                {
                    _steeringWheelObject->rotate(0.0f, -dt * _steerStep * 2 * PI / _maxSteerAngle, 0.0f);
                }
            }

            w->suspension->getBulletConstraint()->setLowerLimit(w->currentAngle);
            w->suspension->getBulletConstraint()->setUpperLimit(w->currentAngle);
        }
    }


}


void BusConstraint::centerSteringWheel(float dt)
{
    float deadZone = 0.0025f;

    WheelList::iterator it = _wheels.begin();

    for (; it != _wheels.end(); ++it)
    {
        Wheel* w = (*it);

        if ( w->steering )
        {
            if (w->currentAngle > deadZone)
            {
                w->currentAngle -= dt * _steerStep;

                w->suspension->getBulletConstraint()->setLowerLimit(w->currentAngle);
                w->suspension->getBulletConstraint()->setUpperLimit(w->currentAngle);

                if (_steeringWheelObject)
                {
                    _steeringWheelObject->rotate(0.0f, dt * _steerStep * 2 * PI / _maxSteerAngle, 0.0f);
                }
            }
            else
            if (w->currentAngle < -deadZone)
            {
                w->currentAngle += dt * _steerStep;

                w->suspension->getBulletConstraint()->setLowerLimit(w->currentAngle);
                w->suspension->getBulletConstraint()->setUpperLimit(w->currentAngle);

                if (_steeringWheelObject)
                {
                    _steeringWheelObject->rotate(0.0f, -dt * _steerStep * 2 * PI / _maxSteerAngle, 0.0f);
                }
            }
        }
    }
}


void BusConstraint::accelerate()
{
    _accelerate = true;
    _brake = false;

    _engine->throttleUp();
}


void BusConstraint::brakeOn()
{
    _accelerate = false;
    _brake = true;

    _engine->throttleDown();
}


void BusConstraint::brakeOff()
{
    _brake = false;
}


void BusConstraint::toggleHandbrake()
{
    _handbrake = !_handbrake;
}

void BusConstraint::startEngine()
{
    if (_engine)
    {
        _engine->turnOn();

        //SoundComponent* sndC = dynamic_cast<SoundComponent*>(_sceneObject->getComponent(CT_SOUND));
        SoundComponent* sndC = dynamic_cast<SoundComponent*>(_modules[0].sceneObject->getComponent(CT_SOUND));
        sndC->play();
    }
}


void BusConstraint::stopEngine()
{
    if (_engine)
    {
        _engine->turnOff();

        //SoundComponent* sndC = dynamic_cast<SoundComponent*>(_sceneObject->getComponent(CT_SOUND));
        SoundComponent* sndC = dynamic_cast<SoundComponent*>(_modules[0].sceneObject->getComponent(CT_SOUND));
        sndC->stop();
    }
}


void BusConstraint::idle()
{
    _accelerate = false;
    _brake = false;

    //_engine->setThrottle(0.0f);
    _engine->throttleDown();
}


Door* BusConstraint::getDoor(unsigned char doorIndex)
{
    if (doorIndex <= _doors.size()-1)
        return _doors[doorIndex];
    else
        return 0;
}


int BusConstraint::getDoorsCount()
{
    return _doors.size();
}


void BusConstraint::doorOpenClose(char doorGroup)
{
    for (unsigned char i = 0; i < _doors.size(); i++)
    //if (doorIndex <= _doors.size()-1)
    {
        if (_doors[i]->getGroup() == doorGroup)
        {
            if (_doors[i]->getState() == EDS_CLOSING)
            {
                _doors[i]->playOpenSound();
                _doors[i]->open();
            }

            else
            if (_doors[i]->getState() == EDS_OPENING)
            {
                _doors[i]->playCloseSound();
                _doors[i]->close();
            }

        }

    }

}


void BusConstraint::update(float dt)
{
    _engine->update(dt);


    //SoundComponent* sndC = dynamic_cast<SoundComponent*>(_sceneObject->getComponent(CT_SOUND));
    SoundComponent* sndC = dynamic_cast<SoundComponent*>(_modules[0].sceneObject->getComponent(CT_SOUND));
    sndC->setPitch(_engine->getCurrentRPM() / 1000);

    btScalar wheelAngularVelocity = 0.0f;

    // przyspieszanie
    if (_accelerate)
    {
        WheelList::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            Wheel* w = (*it);

            if ( w->powered )
            {
                btTransform tmpDirection = w->body->getRigidBody()->getWorldTransform();
                btVector3 forwardDir = tmpDirection.getBasis().getColumn(0);

                w->body->getRigidBody()->applyTorque(forwardDir * _gearbox->currentRatio() * _engine->getCurrentTorque() * 0.0032f );

                //wheelAngularVelocity = w->body->getRigidBody()->getAngularVelocity().length();
            }
        }
    }
    // bus is braking
    else if (_brake)
    {
        WheelList::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            Wheel* w = (*it);

            w->body->getRigidBody()->setDamping(0, w->brakeForce);
        }
    } // bus engine is idle
    else if (!_accelerate && !_brake && !_handbrake)
    {
        WheelList::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            Wheel* w = (*it);

            w->body->getRigidBody()->setDamping(0, 0.13f * abs(_gearbox->currentRatio()) ); // we set a little damping to simulate air and rolling resistance
        }
    } // bus is on handbrake
    else if (_handbrake)
    {
        WheelList::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            Wheel* w = (*it);

            w->body->getRigidBody()->setDamping(0, w->brakeForce);
        }
    }

    char poweredWheels = 0;

    // If we do not accelerate at the moment - we have to calculate and simulate engine braking
    // Get wheel linear speed to calculate RPM of the engine
    WheelList::iterator it = _wheels.begin();
    for (; it != _wheels.end(); ++it)
    {
        Wheel* w = (*it);

        if ( w->powered )
        {
            wheelAngularVelocity += w->body->getRigidBody()->getAngularVelocity().length();
            poweredWheels++;
            //break;
        }
    }

    _engine->setRPM(wheelAngularVelocity/poweredWheels, _gearbox->currentRatio());
}
