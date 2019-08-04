set (PHYSICS_SOURCES
	Physics/Constraint.cpp
	Physics/ConstraintBall.cpp
	Physics/ConstraintHinge.cpp
	Physics/ConstraintHinge2.cpp
	Physics/PhysicalBody.cpp
	Physics/PhysicalBodyBox.cpp
	Physics/PhysicalBodyBvtTriangleMesh.cpp
	Physics/PhysicalBodyConvexHull.cpp
	Physics/PhysicalBodyCylinder.cpp
	Physics/PhysicalBodyRaycastVehicle.cpp
	Physics/PhysicalBodySphere.cpp
	Physics/PhysicalBodyStaticPlane.cpp
	Physics/PhysicalBodyWheel.cpp
	Physics/PhysicsManager.cpp
)

set (PHYSICS_HEADERS 
	Physics/Constraint.hpp
	Physics/ConstraintBall.h
	Physics/ConstraintHinge.hpp
	Physics/ConstraintHinge2.hpp
	Physics/PhysicalBody.hpp
	Physics/PhysicalBodyBox.hpp
	Physics/PhysicalBodyBvtTriangleMesh.hpp
	Physics/PhysicalBodyConvexHull.hpp
	Physics/PhysicalBodyCylinder.hpp
	Physics/PhysicalBodyRaycastVehicle.h
	Physics/PhysicalBodySphere.hpp
	Physics/PhysicalBodyStaticPlane.hpp
	Physics/PhysicalBodyWheel.h
	Physics/PhysicsManager.hpp
)

set (PHYSICS_SOURCES ${PHYSICS_SOURCES} PARENT_SCOPE)
set (PHYSICS_HEADERS ${PHYSICS_HEADERS} PARENT_SCOPE)