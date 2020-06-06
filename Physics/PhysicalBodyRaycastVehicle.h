#ifndef PHYSICALBODYRAYCASTVEHICLE_H_INCLUDED
#define PHYSICALBODYRAYCASTVEHICLE_H_INCLUDED


#include "PhysicalBodyConvexHull.hpp"


const btVector3 WHEEL_DIRECTION = btVector3(0.0f, -1.0f, 0.0f);
const btVector3 WHEEL_AXLE = btVector3(-1.0f, 0.0f, 0.0f);


class PhyscsManager;


class RayCasterWithCollisionGroups : public btVehicleRaycaster
{
    btDynamicsWorld*	m_dynamicsWorld;
    short int           _collisionGroup;
    short int           _collisionFilter;

    public:
        RayCasterWithCollisionGroups(btDynamicsWorld* world)
            :m_dynamicsWorld(world)
        {

        }

        void setCollisionFilter(short int collisionGroup, short int collisionFilter)
        {
            _collisionGroup = collisionGroup;
            _collisionFilter = collisionFilter;
        }

        virtual void* castRay(const btVector3& from,const btVector3& to, btVehicleRaycasterResult& result)
        {
            btCollisionWorld::ClosestRayResultCallback rayCallback(from,to);

            rayCallback.m_collisionFilterMask = _collisionFilter;
            rayCallback.m_collisionFilterGroup = _collisionGroup;

            m_dynamicsWorld->rayTest(from, to, rayCallback);

            if (rayCallback.hasHit())
            {

                const btRigidBody* body = btRigidBody::upcast(rayCallback.m_collisionObject);
                if (body && body->hasContactResponse())
                {
                    result.m_hitPointInWorld = rayCallback.m_hitPointWorld;
                    result.m_hitNormalInWorld = rayCallback.m_hitNormalWorld;
                    result.m_hitNormalInWorld.normalize();
                    result.m_distFraction = rayCallback.m_closestHitFraction;
                    return (void*)body;
                }
            }
            return 0;
        }
};


class PhysicalBodyRaycastVehicle : public PhysicalBodyConvexHull
{
    public:
        PhysicalBodyRaycastVehicle(Vertex* vertices, unsigned int vertexCount, btScalar mass, PhysicsManager* physicsManager);
        PhysicalBodyRaycastVehicle(glm::vec3* vertices, unsigned int vertexCount, btScalar mass, PhysicsManager* physicsManager);
        virtual ~PhysicalBodyRaycastVehicle();

        void setWheelCollisionFilter(short int collisionGroup, short int collisionFilter);

        // return wheel index, do not call manually
        int addWheel(btVector3 connectionPoint, float suspensionRestLength, float radius, bool isFrontWheel);

        btRaycastVehicle* getRayCastVehicle();

    private:
        PhysicsManager* _physMgr;

        btRaycastVehicle::btVehicleTuning _vehicleTuning;
        RayCasterWithCollisionGroups* _rayCaster;
        btRaycastVehicle* _rayCastVehicle;

        virtual void updateBody();
};


#endif // PHYSICALBODYRAYCASTVEHICLE_H_INCLUDED
