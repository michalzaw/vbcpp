#ifndef PHYSICALBODY_HPP_INCLUDED
#define PHYSICALBODY_HPP_INCLUDED

#include <btBulletDynamicsCommon.h>

#include "../Scene/Component.h"
#include "../Scene/SceneObject.h"

#include "../Utils/Math.h"

#include <memory>
#include <set>

class PhysicsManager;
class Constraint;

#define BIT(x) (1<<(x))
enum collisiontypes {
    COL_NOTHING = 0, //<Collide with nothing
    COL_TERRAIN = BIT(0), //<Collide with terrain
    COL_BUS = BIT(1), //<Collide with bus chassis
    COL_DOOR = BIT(2), //<Collide with bus door
    COL_WHEEL = BIT(3), // Collide with bus wheel
    COL_ENV = BIT(4)
};

enum class PhysicalBodyType
{
    BOX,
    BVT_TRIANGLE_MESH,
    CONVEX_HULL,
    CYLINDER,
    GHOST,
    RAYCAST_VEHICLE,
    SPHERE,
    STATIC_PLANE,
    WHEEL
};

class PhysicalBody : public Component
{
    public:
        PhysicalBody(PhysicalBodyType physicalBodyType, btScalar m, bool centerOfMassOffset = false, btVector3 centerOfMassOffsetValue = btVector3(0.0f, 0.0f, 0.0f));
        virtual ~PhysicalBody();

        btRigidBody* getRigidBody() { return _rigidBody.get(); }
        btDefaultMotionState* getMotionState() { return _motionState.get(); }
        btCollisionShape*   getCollisionShape() { return _collShape.get(); }

        btScalar getMass() { return _mass; }

        void setRestitution(btScalar rest) { _rigidBody->setRestitution(rest); }

        inline PhysicalBodyType getPhysicalBodyType() { return _physicalBodyType; }

        void addConstraint(Constraint* c)
        {
            _constraints.push_back(c);
        }

		void removeConstraint(Constraint* c)
		{
			for (std::vector<Constraint*>::iterator i = _constraints.begin(); i != _constraints.end(); ++i)
			{
				if (*i == c)
				{
					_constraints.erase(i);
					break;
				}
			}
		}

		std::vector<Constraint*>& getConstraints()
		{
			return _constraints;
		}

        void setDefaultPosition(btVector3 pos)
        {
            _position = pos;
        }

		void setVisualisationInDebugDrawingStatus(bool enabled);

        virtual void update();


        virtual void changedTransform();

        // functions is called internally by PhysicsManager - do not call manually
        void setCollisionWith(PhysicalBody* body);
        void setNotCollisionWith(PhysicalBody* body);

        inline const std::set<PhysicalBody*>& getObjectsCollidesWith() { return _collidesWith; }
        inline const std::vector<PhysicalBody*>& getObjectsBeginCollision() { return _objectsBeginCollision; }
        inline const std::vector<PhysicalBody*>& getObjectsEndCollision() { return _objectsEndCollision; }

    protected:
        btDiscreteDynamicsWorld*                _dynamicsWorld;
        std::unique_ptr<btRigidBody>            _rigidBody;
        std::unique_ptr<btCollisionShape>       _collShape;
        std::unique_ptr<btDefaultMotionState>   _motionState;
        btScalar                _mass;
        btVector3               _position;      // default position
        btVector3               _oldScale;
		bool					_centerOfMassOffset;
		btVector3				_centerOfMassOffsetValue;

        PhysicalBodyType        _physicalBodyType;

        std::vector<Constraint*> _constraints;

        std::set<PhysicalBody*> _collidesWith;
        std::vector<PhysicalBody*> _objectsBeginCollision;
        std::vector<PhysicalBody*> _objectsEndCollision;

        bool _isUpdateTransformFromObject;

        virtual void updateBody() { }

        virtual void onAttachedToScenObject()
        {
            if (_rigidBody)
                _rigidBody->setUserPointer(this);
        }
};

#endif // PHYSICALBODY_HPP_INCLUDED
