#include "Collision.h"
#include <iostream>


bool isPointInFrustum(Frustum& frustum, glm::vec3 point)
{
    for (int i = 0; i < 6; ++i)
    {
        FrustumPlane plane = (FrustumPlane)i;
        if (frustum.getPlane(plane).a * point.x + frustum.getPlane(plane).b * point.y + frustum.getPlane(plane).c * point.z + frustum.getPlane(plane).d  <= 0)
            return false;
    }

    return true;
}

bool isAABBIntersectFrustum(Frustum& frustum, AABB& aabb)
{
    glm::vec3 point;
    for (int i = 0; i < 6; ++i)
    {
        FrustumPlane plane = (FrustumPlane)i;
        if (frustum.getPlane(plane).a <= 0.0f)
            point.x = aabb.getMinCoords().x;
        else
            point.x = aabb.getMaxCoords().x;

        if (frustum.getPlane(plane).b <= 0.0f)
            point.y = aabb.getMinCoords().y;
        else
            point.y = aabb.getMaxCoords().y;

        if (frustum.getPlane(plane).c <= 0.0f)
            point.z = aabb.getMinCoords().z;
        else
            point.z = aabb.getMaxCoords().z;

        if (frustum.getPlane(plane).a * point.x + frustum.getPlane(plane).b * point.y + frustum.getPlane(plane).c * point.z + frustum.getPlane(plane).d  < 0)
            return false;
    }
    return true;
}


bool isAABBInAABB(AABB& aabb1, AABB& aabb2)
{
    if (aabb2.getMinCoords().x < aabb1.getMinCoords().x ||
        aabb2.getMinCoords().y < aabb1.getMinCoords().y ||
        aabb2.getMinCoords().z < aabb1.getMinCoords().z ||
        aabb2.getMaxCoords().x > aabb1.getMaxCoords().x ||
        aabb2.getMaxCoords().y > aabb1.getMaxCoords().y ||
        aabb2.getMaxCoords().z > aabb1.getMaxCoords().z)
    {
        return false;
    }

    return true;
}


bool isAABBIntersectAABB(AABB& aabb1, AABB& aabb2)
{
    return (aabb1.getMinCoords().x <= aabb2.getMaxCoords().x &&
            aabb1.getMaxCoords().x >= aabb2.getMinCoords().x &&
            aabb1.getMinCoords().y <= aabb2.getMaxCoords().y &&
            aabb1.getMaxCoords().y >= aabb2.getMinCoords().y &&
            aabb1.getMinCoords().z <= aabb2.getMaxCoords().z &&
            aabb1.getMaxCoords().z >= aabb2.getMinCoords().z);
}


bool isAABBInFrustum(Frustum& frustum, AABB& aabb)
{
    glm::vec3 min = aabb.getMinCoords();
    glm::vec3 max = aabb.getMaxCoords();

    if (!isPointInFrustum(frustum, glm::vec3(min.x, min.y, min.z)) ||
        !isPointInFrustum(frustum, glm::vec3(min.x, min.y, max.z)) ||
        !isPointInFrustum(frustum, glm::vec3(min.x, max.y, min.z)) ||
        !isPointInFrustum(frustum, glm::vec3(min.x, max.y, max.z)) ||
        !isPointInFrustum(frustum, glm::vec3(max.x, min.y, min.z)) ||
        !isPointInFrustum(frustum, glm::vec3(max.x, min.y, max.z)) ||
        !isPointInFrustum(frustum, glm::vec3(max.x, max.y, min.z)) ||
        !isPointInFrustum(frustum, glm::vec3(max.x, max.y, max.z)))
    {
        return false;
    }

    return true;
}
