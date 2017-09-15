#ifndef FRUSTUM_H_INCLUDED
#define FRUSTUM_H_INCLUDED


#include <glm/glm.hpp>

#include "AABB.h"


struct Plane
{
    float a;
    float b;
    float c;
    float d;

    Plane()
    {
        a = b = c = d = 0.0f;
    }

    void set(float a, float b, float c, float d)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        this->d = d;
    }
};


enum FrustumPlane
{
    FP_LEFT,
    FP_RIGHT,
    FP_TOP,
    FP_BOTTOM,
    FP_NEAR,
    FP_FAR

};


enum FrustumPoint
{
    FP_NEAR_LEFT_BOTTOM,
    FP_NEAR_RIGHT_BOTTOM,
    FP_NEAR_LEFT_TOP,
    FP_NEAR_RIGHT_TOP,
    FP_FAR_LEFT_BOTTOM,
    FP_FAR_RIGHT_BOTTOM,
    FP_FAR_LEFT_TOP,
    FP_FAR_RIGHT_TOP

};


class Frustum
{
    private:
        Plane _planes[6];
        glm::vec3 _points[8];

        AABB _aabb;

        bool _isCalculatedPoints;

        glm::vec3 intersectPlanes(const Plane& p1, const Plane& p2, const Plane& p3);
        void normalizePlanes();
        void calculatePoints();

    public:
        Frustum();
        Frustum(glm::mat4 matrix);

        void set(glm::mat4 matrix);

        const Plane& getPlane(FrustumPlane plane);
        glm::vec3* const getPoints();

        void applyTransform(const glm::mat4& transform);

        AABB* getAABB();

};


#endif // FRUSTUM_H_INCLUDED
