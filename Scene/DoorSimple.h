#ifndef DOORSIMPLE_H_INCLUDED
#define DOORSIMPLE_H_INCLUDED

#include "Door.h"
#include "../Physics/ConstraintHinge.hpp"

// XML reader
#include "../Utils/tinyxml2.h"
#include <sstream>
#include <cstdlib>
using namespace tinyxml2;

#include "../Graphics/GraphicsManager.h"
#include "../Utils/ResourceManager.h"

class DoorSimple : virtual public Door
{
    public:
        DoorSimple(SceneObject* sceneObject, glm::vec3 relPosition, RModel* model, PhysicalBodyConvexHull* body, ConstraintHinge* hinge, char group = 1)
        : Door(sceneObject, relPosition, model, body, group), _hinge(hinge)
        {
            close();
        }

        DoorSimple(SceneManager* sMgr, PhysicsManager* pMgr, PhysicalBodyConvexHull* chasisBody, glm::vec3 busPosition, std::string busName, std::string texturePath, XMLElement* element)
        :Door(sMgr, pMgr)
        {
            std::string doorName(element->Attribute("name"));
            std::string doorType(element->Attribute("type"));
            std::string doorModel(element->Attribute("model"));
            float mass = (float)atof(element->Attribute("mass"));
            _group = (char)atoi(element->Attribute("group"));

            _relPosition = XMLstringToVec3(element->Attribute("position"));
            glm::vec3 globalPosition = glm::vec3(busPosition + _relPosition);

            btVector3 busPivot = XMLstringToBtVec3(element->Attribute("pivotA"));
            btVector3 doorPivot = XMLstringToBtVec3(element->Attribute("pivotB"));

            _sceneObject = _sMgr->addSceneObject(doorName);

            std::string modelPath = "Buses/" + busName + "/" + doorModel;

            _doorModel = ResourceManager::getInstance().loadModel(modelPath, texturePath);
            RenderObject* doorRender = GraphicsManager::getInstance().addRenderObject(new RenderObject(_doorModel));

            _sceneObject->addComponent(doorRender);

            //btVector3 btDoorPos(globalPosition.x, globalPosition.y, globalPosition.z);
            btVector3 btDoorsPos(_relPosition.x, _relPosition.y, _relPosition.z);

            int collidesWith = COL_ENV | COL_TERRAIN;
            _doorBody = _pMgr->createPhysicalBodyConvexHull(_doorModel->getCollisionMesh(), _doorModel->getCollisionMeshSize(), mass, btVector3(0,0,0), btVector3(0,0,0), COL_DOOR, collidesWith);
            _sceneObject->addComponent(_doorBody);

            _sceneObject->setPosition(_relPosition);

            _hinge = _pMgr->createConstraintHinge(chasisBody, _doorBody, busPivot, doorPivot, btVector3(0,1,0), btVector3(0,1,0));
            _hinge->getBulletConstraint()->setLimit(-1.5,0);



            close();
        }

        virtual ~DoorSimple()
        {
            _pMgr->removeConstraint(_hinge);
        }

        ConstraintHinge* getConstraint() { return _hinge; }

        void open()
        {
            _hinge->getBulletConstraint()->enableAngularMotor(true, -1.9f, 0.15f);
            _state = EDS_OPENING;
        }

        void close()
        {
            _hinge->getBulletConstraint()->enableAngularMotor(true, 1.9f, 0.15f);
            _state = EDS_CLOSING;
        }

    protected:
        ConstraintHinge*        _hinge;
};


#endif // DOORSIMPLE_H_INCLUDED
