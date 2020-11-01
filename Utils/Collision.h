#ifndef COLLISION_H_INCLUDED
#define COLLISION_H_INCLUDED


#include "../Graphics/Frustum.h"
#include "../Graphics/AABB.h"


bool isPointInFrustum(Frustum& frustum, glm::vec3 point);
bool isAABBIntersectFrustum(Frustum& frustum, AABB& aabb);
bool isAABBInAABB(AABB& aabb1, AABB& aabb2);
bool isAABBIntersectAABB(AABB& aabb1, AABB& aabb2);
bool isAABBInFrustum(Frustum& frustum, AABB& aabb);

// http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
// this function does not work with scaled objects
bool isRayIntersectOBB(glm::vec3 rayOrigin, glm::vec3 rayDirection, AABB& aabb, glm::mat4 modelMatrix, float& intersection_distance);

// https://asawicki.info/productions/artykuly/Zaawansowana_kamera_3D.php5
bool collisionRayToPlane(const glm::vec3& rayOrigin, const glm::vec3& rayDirection, const Plane& plane, float& outT, float& outVD);

#endif // COLLISION_H_INCLUDED
