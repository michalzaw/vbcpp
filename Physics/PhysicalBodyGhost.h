#ifndef PHYSICALBODYGHOST_HPP_INCLUDED
#define PHYSICALBODYGHOST_HPP_INCLUDED


#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "PhysicalBody.hpp"


class PhysicalBodyGhost : public PhysicalBody
{
    public:
        PhysicalBodyGhost(const btVector3& size);
        virtual ~PhysicalBodyGhost();

        void setSize(const btVector3& size);
        const btVector3& getSize();

        void update() override;
        void changedTransform() override;

        inline btGhostObject* getBulletObject() { return _ghostObject; }

    protected:
        void onAttachedToScenObject() override;

    private:
        btVector3 _size;

        btGhostObject* _ghostObject;

        SceneObject* _helperSceneObject;

        void updateBody();
};

#endif // PHYSICALBODYCYLINDER_HPP_INCLUDED
