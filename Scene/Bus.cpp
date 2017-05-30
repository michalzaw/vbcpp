#include "Bus.h"

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

Bus::Bus(SceneManager* smgr, PhysicsManager* pmgr, SoundManager* sndMgr, std::string filename)
: _sMgr(smgr), _pMgr(pmgr), _sndMgr(sndMgr), _sceneObject(0), _chasisBody(0),
_maxSteerAngle(0.55f), _steerStep(0.5f),
_brake(false), _accelerate(false),
_brakeForce(0.0f), _brakeForceStep(0.2f),
_steeringWheelObject(NULL), _driverPosition(0.0f, 0.0f, 0.0f),
_isEnableLights(false), _isEnableHeadlights(false),
_collidesWith(COL_TERRAIN | COL_ENV)
{
    std::cout << "Bus Konstruktor" << std::endl;

    loadXMLdata(filename);
}

Bus::~Bus()
{
    std::cout << "Bus Destruktor" << std::endl;

    if (_gearbox)
    {
        delete _gearbox;
        _gearbox = 0;
    }

    if (_engine)
    {
        delete _engine;
        _engine = 0;
    }

    WheelList::iterator wit = _wheels.begin();

    for (; wit != _wheels.end(); ++wit)
        delete (*wit);

    DoorList::iterator dit = _doors.begin();

    for (; dit != _doors.end(); ++dit)
        delete (*dit);

    _headlights.clear();
}


void Bus::loadXMLdata(std::string busname)
{
    std::string filename = "Buses/" + busname + "/config.xml";

    XMLDocument doc;
    doc.LoadFile( filename.c_str() );

    XMLElement* objElement = doc.FirstChildElement("Gearbox");

    std::string gearboxFile(objElement->Attribute("model"));
    _gearbox = new Gearbox(gearboxFile);

    objElement = doc.FirstChildElement("Engine");
    std::string engineFile(objElement->Attribute("model"));
    _engine = new Engine(engineFile);

    objElement = doc.FirstChildElement("Object");

    std::cout << "XML DATA" << std::endl;

    std::string sObjName(objElement->Attribute("name"));

    std::cout<< sObjName << std::endl;

    RModel* busModel = 0;

    glm::vec3 busPosition = glm::vec3(0,0,0);
    glm::vec3 busRotation = glm::vec3(0,0,0);

    std::string texturePath = "Buses/" + busname + "/";

    _sceneObject = _sMgr->addSceneObject(sObjName);


    // Create Sound Component if sound filename is defined in Engine XML config file
    if (_engine->getSoundFilename() != "")
    {
        SoundComponent* soundComp = new SoundComponent(_engine->getSoundFilename(), EST_PLAYER, true);
        _sceneObject->addComponent(soundComp);
        //soundComp->setGain(0.6f);

        _sndMgr->addSoundComponent(soundComp);
    }

    for (XMLElement* child = objElement->FirstChildElement(); child != NULL; child = child->NextSiblingElement())
    {
        const char* ename = child->Name();

        // OBJECT TRANSFORM
        if (strcmp(ename,"transform") == 0)
        {
            //std::cout << "XML: Transform" << std::endl;

            const char* cPosition = child->Attribute("position");
            busPosition = XMLstringToVec3(cPosition);

            const char* cRotation = child->Attribute("rotation");
            busRotation = XMLstringToVec3(cRotation);

            const char* cScale = child->Attribute("scale");
            glm::vec3 scale(XMLstringToVec3(cScale));
            _sceneObject->setScale(scale);

        }
        else // BODY DATA
        if (strcmp(ename,"body") == 0)
        {
            //std::cout << "XML: Body data" << std::endl;

            std::string modelFile = std::string(child->Attribute("model"));
            std::string modelPath = "Buses/" + busname + "/" + modelFile;
            texturePath += std::string(child->Attribute("textures")) + "/";

            busModel = ResourceManager::getInstance().loadModel(modelPath, texturePath);
            RenderObject* renderObj = GraphicsManager::getInstance().addRenderObject(new RenderObject(busModel));

            _sceneObject->addComponent(renderObj);

            // Tworzenie fizycznego obiektu karoserii
            const char* cMass = child->Attribute("mass");
            float fMass = (float)atof(cMass);

            btVector3 btPos(busPosition.x, busPosition.y, busPosition.z);


            if (busModel->getCollisionMeshSize() > 0)
            {
                _chasisBody = _pMgr->createPhysicalBodyConvexHull(busModel->getCollisionMesh(), busModel->getCollisionMeshSize(), fMass, COL_BUS, _collidesWith);
                _chasisBody->getRigidBody()->setActivationState( DISABLE_DEACTIVATION );
                _sceneObject->addComponent(_chasisBody);

                btVector3 dirVec(0,0,1);
                btTransform tmpTransf = _chasisBody->getRigidBody()->getWorldTransform();
                btVector3 dir = tmpTransf.getBasis() * dirVec;
            }
            else
            {
                std::cout << "ERROR: Collision mesh not found in bus model!\n" << std::endl;
                return;
            }

        }
        else // WHEEL DATA
        if (strcmp(ename,"wheel") == 0)
        {
            std::cout << "XML: Wheel data" << std::endl;

            std::string wheelName(child->Attribute("name"));
            std::string wheelModel(child->Attribute("model"));
            std::string side(child->Attribute("side"));
            float mass = (float)atof(child->Attribute("mass"));
            float radius = (float)atof(child->Attribute("radius"));
            float width = (float)atof(child->Attribute("width"));

            WheelSide wheelSide;

            int steering = (int)atoi(child->Attribute("steering"));
            int powered = (int)atoi(child->Attribute("powered"));

            float stiffness = (float)atof(child->Attribute("stiffness"));
            float damping = (float)atof(child->Attribute("damping"));
            float brakeForce = (float)atof(child->Attribute("brakeForce"));

            SceneObject* wheelObj = _sMgr->addSceneObject(wheelName);


            glm::vec3 wheelPosition = XMLstringToVec3(child->Attribute("position"));
            glm::vec3 relativePos = _chasisBody->getSceneObject()->transformLocalPointToGlobal(wheelPosition);


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
            RModel* wheel = ResourceManager::getInstance().loadModel(modelPath, texturePath);
            RenderObject* wheelRender = GraphicsManager::getInstance().addRenderObject(new RenderObject(wheel));

            wheelObj->addComponent(wheelRender);

            int collidesWith = COL_TERRAIN | COL_ENV;
            PhysicalBodyCylinder* wheelCyl = _pMgr->createPhysicalBodyCylinder(btVector3(width, radius, radius), mass, X_AXIS, COL_WHEEL, collidesWith);
            wheelCyl->getRigidBody()->setFriction(10.0f);
            wheelCyl->getRigidBody()->setRestitution(0.1f);
            wheelObj->addComponent(wheelCyl);

            ConstraintHinge2* hinge1 = _pMgr->createConstraintHinge2(_chasisBody, wheelCyl, btWheelPos, btVector3(0,1,0), btVector3(1,0,0));
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
        }
        else // DOOR DATA
        if (strcmp(ename,"door") == 0)
        {
            std::cout << "XML: Door data" << std::endl;

            std::string doorName(child->Attribute("name"));
            std::string doorType(child->Attribute("type"));
            std::string doorModel(child->Attribute("model"));
            float mass = (float)atof(child->Attribute("mass"));
            char group = (char)atoi(child->Attribute("group"));

            std::string openSound = "Buses/" + busname + "/" + std::string(child->Attribute("doorOpenSound"));
            std::string closeSound = "Buses/" + busname + "/" + std::string(child->Attribute("doorCloseSound"));

            std::cout << "Door open sound:" << openSound << std::endl;
            std::cout << "Door open sound:" << closeSound <<std::endl;


            // Create sound component
            SoundComponent* openSoundComp = new SoundComponent(openSound, EST_PLAYER);
            _sndMgr->addSoundComponent(openSoundComp);

            SoundComponent* closeSoundComp = new SoundComponent(closeSound, EST_PLAYER);
            _sndMgr->addSoundComponent(closeSoundComp);

            SceneObject* doorObj = 0;

            if (doorType == "s")
            {

                glm::vec3 doorPosition = XMLstringToVec3(child->Attribute("position"));
                //glm::vec3 relativePos = glm::vec3(busPosition.x + doorPosition.x, busPosition.y + doorPosition.y, busPosition.z + doorPosition.z);
                glm::vec3 relativePos = _chasisBody->getSceneObject()->transformLocalPointToGlobal(doorPosition);

                // poczatek IF
                btVector3 busPivot = XMLstringToBtVec3(child->Attribute("pivotA"));
                btVector3 doorPivot = XMLstringToBtVec3(child->Attribute("pivotB"));

                doorObj = _sMgr->addSceneObject(doorName);
                doorObj->setPosition(doorPosition);

                std::string modelPath = "Buses/" + busname + "/" + doorModel;

                RModel* dr = ResourceManager::getInstance().loadModel(modelPath, texturePath);
                RenderObject* doorRender = GraphicsManager::getInstance().addRenderObject(new RenderObject(dr));

                doorObj->addComponent(doorRender);

                btVector3 btDoorPos(relativePos.x, relativePos.y, relativePos.z);

                int collidesWith = COL_ENV | COL_TERRAIN;
                PhysicalBodyConvexHull* doorBody = _pMgr->createPhysicalBodyConvexHull(dr->getCollisionMesh(), dr->getCollisionMeshSize(), mass, COL_DOOR, collidesWith);
                doorObj->addComponent(doorBody);
                //doorBody->_position = btVector3(doorPosition.x, doorPosition.y, doorPosition.z);

                ConstraintHinge* doorHinge = _pMgr->createConstraintHinge(_chasisBody, doorBody, busPivot, doorPivot, btVector3(0,1,0), btVector3(0,1,0));

                doorHinge->getBulletConstraint()->setLimit(-1.5,0);

                Door* d = 0;
                d = new DoorSimple(dr, doorBody, doorHinge, openSoundComp, closeSoundComp, group);
                _doors.push_back(d);
            } // IF "S"
            else
            if (doorType == "se")
            {
                std::string armName(child->Attribute("armName"));
                std::string armModel(child->Attribute("arm"));
                float armMass = (float)atof(child->Attribute("armMass"));
                char group = (char)atoi(child->Attribute("group"));

                float arm1lowLimit = (float)atof(child->Attribute("arm1lowLimit"));
                float arm1highLimit = (float)atof(child->Attribute("arm1highLimit"));
                glm::vec2 arm1limits = XMLstringToVec2(child->Attribute("arm1limits"));

                std::string rotDir(child->Attribute("rotationDir"));

                RotationDir rdir;

                if (rotDir == "CCW")
                    rdir = ERD_CCW;
                else
                    rdir = ERD_CW;

                glm::vec3 armPosition = XMLstringToVec3(child->Attribute("armPosition"));
                glm::vec3 armRelPos = glm::vec3(busPosition.x + armPosition.x, busPosition.y + armPosition.y, busPosition.z + armPosition.z);

                btVector3 armPivotA = XMLstringToBtVec3(child->Attribute("armPivotA"));
                btVector3 armPivotB = XMLstringToBtVec3(child->Attribute("armPivotB"));

                SceneObject* armObj = _sMgr->addSceneObject(armName);
                armObj->setPosition(armRelPos);

                std::string armPath = "Buses/" + busname + "/" + armModel;

                RModel* arm = ResourceManager::getInstance().loadModel(armPath, texturePath);
                RenderObject* armRender = GraphicsManager::getInstance().addRenderObject(new RenderObject(arm));
                armObj->addComponent(armRender);

                btVector3 btArmPos(armRelPos.x, armRelPos.y, armRelPos.z);

                int collidesWith = COL_ENV | COL_TERRAIN;
                PhysicalBodyConvexHull* armBody = _pMgr->createPhysicalBodyConvexHull(arm->getCollisionMesh(), arm->getCollisionMeshSize(), armMass, COL_DOOR, collidesWith);
                armObj->addComponent(armBody);

                ConstraintHinge* busArmHinge = _pMgr->createConstraintHinge(_chasisBody, armBody, armPivotA, armPivotB, btVector3(0,1,0), btVector3(0,1,0));

                busArmHinge->getBulletConstraint()->setLimit(arm1limits.x,arm1limits.y);

                // arm 2

                std::string arm2Name(child->Attribute("arm2Name"));
                std::string arm2Model(child->Attribute("arm2"));
                float arm2Mass = (float)atof(child->Attribute("arm2Mass"));

                float arm2lowLimit = (float)atof(child->Attribute("arm2lowLimit"));
                float arm2highLimit = (float)atof(child->Attribute("arm2highLimit"));

                glm::vec3 arm2Position = XMLstringToVec3(child->Attribute("arm2Position"));
                glm::vec3 arm2RelPos = glm::vec3(busPosition.x + arm2Position.x, busPosition.y + arm2Position.y, busPosition.z + arm2Position.z);

                btVector3 arm2PivotA = XMLstringToBtVec3(child->Attribute("arm2PivotA"));
                btVector3 arm2PivotB = XMLstringToBtVec3(child->Attribute("arm2PivotB"));

                SceneObject* arm2Obj = _sMgr->addSceneObject(arm2Name);
                arm2Obj->setPosition(arm2RelPos);

                std::string arm2Path = "Buses/" + busname + "/" + arm2Model;

                RModel* arm2 = ResourceManager::getInstance().loadModel(arm2Path, texturePath);
                RenderObject* arm2Render = GraphicsManager::getInstance().addRenderObject(new RenderObject(arm2));
                arm2Obj->addComponent(arm2Render);

                btVector3 btArm2Pos(arm2RelPos.x, arm2RelPos.y, arm2RelPos.z);

                collidesWith = COL_NOTHING;
                PhysicalBodyConvexHull* arm2Body = _pMgr->createPhysicalBodyConvexHull(arm2->getCollisionMesh(), arm2->getCollisionMeshSize(), arm2Mass, COL_DOOR, collidesWith);
                arm2Obj->addComponent(arm2Body);

                ConstraintHinge* busArm2Hinge = _pMgr->createConstraintHinge(_chasisBody, arm2Body, arm2PivotA, arm2PivotB, btVector3(0,1,0), btVector3(0,1,0));

                //busArm2Hinge->getBulletConstraint()->setLimit(arm2lowLimit,arm2highLimit);

                // door model

                std::string doorName(child->Attribute("name"));
                std::string doorModel(child->Attribute("model"));
                float doorMass = (float)atof(child->Attribute("mass"));

                glm::vec3 doorPosition = XMLstringToVec3(child->Attribute("position"));
                glm::vec3 relativePos = glm::vec3(armPosition.x + doorPosition.x, armPosition.y + doorPosition.y, armPosition.z + doorPosition.z);

                btVector3 doorPivotA = XMLstringToBtVec3(child->Attribute("pivotA"));
                btVector3 doorPivotB = XMLstringToBtVec3(child->Attribute("pivotB"));

                doorObj = _sMgr->addSceneObject(doorName);
                doorObj->setPosition(relativePos);

                std::string doorPath = "Buses/" + busname + "/" + doorModel;

                RModel* door = ResourceManager::getInstance().loadModel(doorPath, texturePath);
                RenderObject* doorRender = GraphicsManager::getInstance().addRenderObject(new RenderObject(door));
                doorObj->addComponent(doorRender);

                btVector3 btDoorPos(relativePos.x, relativePos.y, relativePos.z);

                collidesWith = COL_ENV;
                PhysicalBodyConvexHull* doorBody = _pMgr->createPhysicalBodyConvexHull(door->getCollisionMesh(), door->getCollisionMeshSize(), doorMass, COL_DOOR, collidesWith);
                doorObj->addComponent(doorBody);

                ConstraintHinge* armDoorHinge = _pMgr->createConstraintHinge(armBody, doorBody, doorPivotA, doorPivotB, btVector3(0,1,0), btVector3(0,1,0));

                if (rotDir == "CCW")
                    armDoorHinge->getBulletConstraint()->setLimit(-1.9,0.0);
                else
                    armDoorHinge->getBulletConstraint()->setLimit(0.0,1.9);

                btVector3 pivotC = XMLstringToBtVec3(child->Attribute("pivotC"));
                btVector3 pivotD = XMLstringToBtVec3(child->Attribute("pivotD"));

                ConstraintHinge* arm2doorHinge = _pMgr->createConstraintHinge(arm2Body, doorBody, pivotC, pivotD, btVector3(0,1,0), btVector3(0,1,0));


                Door* d = 0;
                d = new DoorSE(0, 0, arm, armBody, busArmHinge, 0, openSoundComp, closeSoundComp, rdir, group);
                _doors.push_back(d);
            }


            doorObj->addComponent(openSoundComp);
            doorObj->addComponent(closeSoundComp);

            // koniec IF
        }
        else // STERING WHEEL
        if (strcmp(ename,"steering_wheel") == 0)
        {
            //std::cout << "XML: Steering wheel" << std::endl;

            std::string modelFile = std::string(child->Attribute("model"));
            std::string modelPath = "Buses/" + busname + "/" + modelFile;

            _steeringWheelObject = _sMgr->addSceneObject("steeringWheel");

            RModel* steeringWheelModel = ResourceManager::getInstance().loadModel(modelPath, "./");
            RenderObject* renderObj = GraphicsManager::getInstance().addRenderObject(new RenderObject(steeringWheelModel));

            _steeringWheelObject->addComponent(renderObj);


            const char* cPosition = child->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);
            _steeringWheelObject->setPosition(position);

            const char* cRotation = child->Attribute("rotation");
            glm::vec3 rotation(XMLstringToVec3(cRotation));
            _steeringWheelObject->setRotation(glm::vec3(rotation.x * PI, rotation.y * PI, rotation.z * PI) );

            const char* cScale = child->Attribute("scale");
            glm::vec3 scale(XMLstringToVec3(cScale));
            _steeringWheelObject->setScale(scale);

            _sceneObject->addChild(_steeringWheelObject);
        }
        else // DRIVER
        if (strcmp(ename,"driver") == 0)
        {
            //std::cout << "XML: Driver" << std::endl;

            const char* cPosition = child->Attribute("position");
            _driverPosition = XMLstringToVec3(cPosition);
        }
        else // LIGHT
        if (strcmp(ename,"light") == 0)
        {
            //std::cout << "XML: Light" << std::endl;

            const char* cPosition = child->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);

            const char* cColor = child->Attribute("color");
            glm::vec3 color = XMLstringToVec3(cColor);

            const char* cAttenuation = child->Attribute("attenuation");
            glm::vec3 attenuation = XMLstringToVec3(cAttenuation);

            float ambientIntensity = (float)atof(child->Attribute("ambientIntensity"));
            float diffuseIntensity = (float)atof(child->Attribute("diffuseIntensity"));


            SceneObject* light = _sMgr->addSceneObject("busLight" + toString(_lights.size()));
            Light* lightComponent = GraphicsManager::getInstance().addPointLight(color, ambientIntensity,
                                                                                 diffuseIntensity,
                                                                                 LightAttenuation(attenuation.x, attenuation.y, attenuation.z));
            light->addComponent(lightComponent);
            light->setPosition(position);
            _sceneObject->addChild(light);

            lightComponent->setIsActive(_isEnableLights);

            _lights.push_back(lightComponent);
        }
        else // HEADLIGHTS
        if (strcmp(ename,"headlights") == 0)
        {
            //std::cout << "XML: Headlights" << std::endl;

            std::string headlightName(child->Attribute("name"));
            const char* cPosition = child->Attribute("position");
            glm::vec3 position = XMLstringToVec3(cPosition);

            const char* cRotation = child->Attribute("rotation");
            glm::vec3 rotation = XMLstringToVec3(cRotation);

            const char* cColor = child->Attribute("color");
            glm::vec3 color = XMLstringToVec3(cColor);

            const char* cAttenuation = child->Attribute("attenuation");
            glm::vec3 attenuation = XMLstringToVec3(cAttenuation);

            float ambientIntensity = (float)atof(child->Attribute("ambientIntensity"));
            float diffuseIntensity = (float)atof(child->Attribute("diffuseIntensity"));
            float cutoff = (float)atof(child->Attribute("cutoff"));


            SceneObject* light = _sMgr->addSceneObject(headlightName);
            Light* lightComponent = GraphicsManager::getInstance().addSpotLight(color, ambientIntensity,
                                                                                diffuseIntensity, cutoff,
                                                                                LightAttenuation(attenuation.x, attenuation.y, attenuation.z));
            light->addComponent(lightComponent);
            light->setPosition(position);
            light->setRotation(rotation);
            _sceneObject->addChild(light);

            lightComponent->setIsActive(_isEnableHeadlights);

            _headlights.push_back(lightComponent);
        }
    }
    _sceneObject->setPosition(busPosition);
    _sceneObject->setRotation(glm::vec3(busRotation.x, busRotation.y, busRotation.z) );

}

glm::vec3 Bus::getDriverPosition()
{
    return _driverPosition;
}


void Bus::setIsEnableLights(bool is)
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


bool Bus::isEnableLights()
{
    return _isEnableLights;
}


void Bus::setIsEnableHeadlights(bool is)
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


bool Bus::isEnableHeadlights()
{
    return _isEnableHeadlights;
}


void Bus::turnLeft(float dt)
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


void Bus::turnRight(float dt)
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


void Bus::centerSteringWheel(float dt)
{
WheelList::iterator it = _wheels.begin();

    for (; it != _wheels.end(); ++it)
    {
        Wheel* w = (*it);

        if ( w->steering )
        {
            if (w->currentAngle > 0)
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
            if (w->currentAngle < 0)
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


void Bus::accelerate()
{
    _accelerate = true;
    _brake = false;

    _engine->throttleUp();
}


void Bus::brakeOn()
{
    _accelerate = false;
    _brake = true;

    _engine->throttleDown();
}


void Bus::brakeOff()
{
    _brake = false;
}


void Bus::startEngine()
{
    if (_engine)
    {
        _engine->turnOn();

        SoundComponent* sndC = dynamic_cast<SoundComponent*>(_sceneObject->getComponent(CT_SOUND));
        sndC->play();
    }
}


void Bus::stopEngine()
{
    if (_engine)
    {
        _engine->turnOff();

        SoundComponent* sndC = dynamic_cast<SoundComponent*>(_sceneObject->getComponent(CT_SOUND));
        sndC->stop();
    }
}


void Bus::idle()
{
    _accelerate = false;
    _brake = false;

    _engine->setThrottle(0.0f);
}


Door* Bus::getDoor(unsigned char doorIndex)
{
    if (doorIndex <= _doors.size()-1)
        return _doors[doorIndex];
    else
        return 0;
}


void Bus::doorOpenClose(char doorGroup)
{
    for (char i = 0; i < _doors.size(); i++)
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


void Bus::updatePhysics(float dt)
{
    _engine->update(dt);


    SoundComponent* sndC = dynamic_cast<SoundComponent*>(_sceneObject->getComponent(CT_SOUND));
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
    // hamowanie
    else if (_brake)
    {
        WheelList::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            Wheel* w = (*it);

            w->body->getRigidBody()->setDamping(0, w->brakeForce);
        }
    }
    else if (!_accelerate && !_brake)
    {
        WheelList::iterator it = _wheels.begin();

        for (; it != _wheels.end(); ++it)
        {
            Wheel* w = (*it);

            w->body->getRigidBody()->setDamping(0, 0.13f * abs(_gearbox->currentRatio()) );
        }
    }

    char poweredWheels = 0;

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

    //if (_gearbox->currentRatio() != 0.0f)
    _engine->setRPM(wheelAngularVelocity/poweredWheels, _gearbox->currentRatio());
}
