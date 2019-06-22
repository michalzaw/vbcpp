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


// http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-custom-ray-obb-function/
bool isRayIntersectOBB(glm::vec3 rayOrigin, glm::vec3 rayDirection, AABB& aabb, glm::mat4 modelMatrix, float& intersectionDistance)
{
    float min = 0.0f;
    float max = 100000.0f;

    glm::vec3 obbPositionWorldspace(modelMatrix[3].x, modelMatrix[3].y, modelMatrix[3].z);

    glm::vec3 deltaPosition = obbPositionWorldspace - rayOrigin;

    {
        glm::vec3 xAxis(modelMatrix[0].x, modelMatrix[0].y, modelMatrix[0].z);
        float e = glm::dot(xAxis, deltaPosition);
        float f = glm::dot(rayDirection, xAxis);

        if (fabs(f) > 0.001f)
        {
            float t1 = (e + aabb.getMinCoords().x) / f;
            float t2 = (e + aabb.getMaxCoords().x) / f;

            if (t1 > t2)
            {
                float w = t1;
                t1 = t2;
                t2 = w;
            }

            if (t2 < max)
                max = t2;
            if (t1 > min)
                min = t1;

            if (max < min)
                return false;
        }
        else
        {
            if (-e + aabb.getMinCoords().x > 0.0f || -e + aabb.getMaxCoords().x < 0.0f)
                return false;
        }
    }

    {
        glm::vec3 yAxis(modelMatrix[1].x, modelMatrix[1].y, modelMatrix[1].z);
        float e = glm::dot(yAxis, deltaPosition);
        float f = glm::dot(rayDirection, yAxis);

        if (fabs(f) > 0.001f)
        {
            float t1 = (e + aabb.getMinCoords().y) / f;
            float t2 = (e + aabb.getMaxCoords().y) / f;

            if (t1 > t2)
            {
                float w = t1;
                t1 = t2;
                t2 = w;
            }

            if (t2 < max)
                max = t2;
            if (t1 > min)
                min = t1;

            if (max < min)
                return false;
        }
        else
        {
            if (-e + aabb.getMinCoords().y > 0.0f || -e + aabb.getMaxCoords().y < 0.0f)
                return false;
        }
    }

    {
        glm::vec3 zAxis(modelMatrix[2].x, modelMatrix[2].y, modelMatrix[2].z);
        float e = glm::dot(zAxis, deltaPosition);
        float f = glm::dot(rayDirection, zAxis);

        if (fabs(f) > 0.001f)
        {
            float t1 = (e + aabb.getMinCoords().z) / f;
            float t2 = (e + aabb.getMaxCoords().z) / f;

            if (t1 > t2)
            {
                float w = t1;
                t1 = t2;
                t2 = w;
            }

            if (t2 < max)
                max = t2;
            if (t1 > min)
                min = t1;

            if (max < min)
                return false;
        }
        else
        {
            if (-e + aabb.getMinCoords().z > 0.0f || -e + aabb.getMaxCoords().z < 0.0f)
                return false;
        }
    }

    intersectionDistance = min;

    return true;
}
