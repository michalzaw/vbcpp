#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED


#include "../Graphics/Frustum.h"
#include "../Graphics/AABB.h"


bool isPointInFrustum(Frustum& frustum, glm::vec3 point);
bool isAABBIntersectFrustum(Frustum& frustum, AABB& aabb);
bool isAABBInAABB(AABB& aabb1, AABB& aabb2);
bool isAABBInFrustum(Frustum& frustum, AABB& aabb);


#endif // COLLISION_H_INCLUDED
