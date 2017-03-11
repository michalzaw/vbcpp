#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED

#include "../Utils/RModel.h"
#include "../Physics/PhysicalBodyConvexHull.hpp"

#include <glm/gtx/matrix_decompose.hpp>

#include "SceneManager.h"
#include "../Physics/PhysicsManager.hpp"


enum DoorState
{
    EDS_OPENING = 0,
    EDS_CLOSING = 1
};


class Door
{
    public:
        Door(SceneObject* sceneObject, glm::vec3 relPosition, RModel* model, PhysicalBodyConvexHull* body, char group = 1)
        : _sceneObject(sceneObject), _relPosition(relPosition), _doorBody(body), _doorModel(model), _state(EDS_CLOSING), _collidesWith(COL_NOTHING), _group(group)
        {

        }

        Door(SceneManager* sMgr, PhysicsManager* pMgr)
        : _sMgr(sMgr), _pMgr(pMgr)
        {

        }

        virtual ~Door()
        {
            printf("DOOR: USUWANIE SCENE OBJECT: %s\n", _sceneObject->getName().c_str());

            _sMgr->removeSceneObject(_sceneObject);

            if (_doorBody)
            {
                std::cout << "Physical body pointer: " << _doorBody << std::endl;
                //_pMgr->removePhysicalBody(_doorBody);
                _doorBody = 0;
            }

        }

        virtual void open() = 0;
        virtual void close() = 0;

        PhysicalBodyConvexHull* getDoorBody() { return _doorBody; }

        RModel* getDoorModel() { return _doorModel; }

        char getGroup() { return _group; }

        SceneObject* getSceneObject() { return _sceneObject; }

        DoorState getState() { return _state; }

        glm::vec3 getRelativePosition() { return _relPosition; }

        void setRelativePosition(glm::mat4 matParent)
        {
            glm::vec3 scale;
            glm::quat rotation;
            glm::vec3 translation;
            glm::vec3 skew;
            glm::vec4 perspective;
            glm::decompose(matParent, scale, rotation, translation, skew, perspective);

            glm::mat4 pos = glm::translate(translation + _relPosition);

            glm::mat4 rot = glm::mat4_cast(rotation);



            glm::mat4 s = glm::scale(glm::vec3(1,1,1));

            _sceneObject->setLocalTransformMatrix(pos * rot * s);
        }

        protected:
            char                     _group;
            SceneObject*             _sceneObject;
            RModel*                  _doorModel;
            PhysicalBodyConvexHull*  _doorBody;
            DoorState                _state;
            int                      _collidesWith;
            glm::vec3                _relPosition;

            SceneManager*            _sMgr;
            PhysicsManager*          _pMgr;
};



#endif // DOOR_H_INCLUDED
