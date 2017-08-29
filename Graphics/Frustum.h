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


glm::vec3 intersectPlanes(const Plane& p1, const Plane& p2, const Plane& p3);


class Frustum
{
    private:
        Plane _planes[6];
        glm::vec3 _points[8];

        AABB _aabb;

        bool _isCalculatedPoints;

        void normalizePlanes()
        {
            for (int i = 0; i < 6; ++i)
            {
                float n = sqrtf(_planes[i].a * _planes[i].a + _planes[i].b * _planes[i].b + _planes[i].c * _planes[i].c);

                _planes[i].a /= n;
                _planes[i].b /= n;
                _planes[i].c /= n;
                _planes[i].d /= n;
            }
        }

        void calculatePoints()
        {
            _points[FP_NEAR_LEFT_BOTTOM] = intersectPlanes(_planes[FP_NEAR], _planes[FP_LEFT], _planes[FP_BOTTOM]);
            _points[FP_NEAR_RIGHT_BOTTOM] = intersectPlanes(_planes[FP_NEAR], _planes[FP_RIGHT], _planes[FP_BOTTOM]);
            _points[FP_NEAR_LEFT_TOP] = intersectPlanes(_planes[FP_NEAR], _planes[FP_LEFT], _planes[FP_TOP]);
            _points[FP_NEAR_RIGHT_TOP] = intersectPlanes(_planes[FP_NEAR], _planes[FP_RIGHT], _planes[FP_TOP]);
            _points[FP_FAR_LEFT_BOTTOM] = intersectPlanes(_planes[FP_FAR], _planes[FP_LEFT], _planes[FP_BOTTOM]);
            _points[FP_FAR_RIGHT_BOTTOM] = intersectPlanes(_planes[FP_FAR], _planes[FP_RIGHT], _planes[FP_BOTTOM]);
            _points[FP_FAR_LEFT_TOP] = intersectPlanes(_planes[FP_FAR], _planes[FP_LEFT], _planes[FP_TOP]);
            _points[FP_FAR_RIGHT_TOP] = intersectPlanes(_planes[FP_FAR], _planes[FP_RIGHT], _planes[FP_TOP]);
        }

    public:
        Frustum()
        {
            _isCalculatedPoints = false;
        }

        Frustum(glm::mat4 matrix)
        {
            _isCalculatedPoints = false;

            set(matrix);
        }

        void set(glm::mat4 matrix)
        {
            _isCalculatedPoints = false;

            /*_planes[FP_LEFT].set(matrix[3] + matrix[0], matrix[7] + matrix[4], matrix[11] + matrix[8], matrix[15] + matrix[12]);
            _planes[FP_RIGHT].set(matrix[3] - matrix[0], matrix[7] - matrix[4], matrix[11] - matrix[8], matrix[15] - matrix[12]);
            _planes[FP_TOP].set(matrix[3] - matrix[1], matrix[7] - matrix[5], matrix[11] - matrix[9], matrix[15] - matrix[13]);
            _planes[FP_BOTTOM].set(matrix[3] + matrix[1], matrix[7] + matrix[5], matrix[11] + matrix[9], matrix[15] + matrix[13]);
            _planes[FP_NEAR].set(matrix[3] + matrix[2], matrix[7] + matrix[6], matrix[11] + matrix[10], matrix[15] + matrix[14]);
            _planes[FP_FAR].set(matrix[3] - matrix[2], matrix[7] - matrix[6], matrix[11] - matrix[10], matrix[15] - matrix[14]);*/

            _planes[FP_LEFT].set(matrix[0][3] + matrix[0][0], matrix[1][3] + matrix[1][0], matrix[2][3] + matrix[2][0], matrix[3][3] + matrix[3][0]);
            _planes[FP_RIGHT].set(matrix[0][3] - matrix[0][0], matrix[1][3] - matrix[1][0], matrix[2][3] - matrix[2][0], matrix[3][3] - matrix[3][0]);
            _planes[FP_TOP].set(matrix[0][3] - matrix[0][1], matrix[1][3] - matrix[1][1], matrix[2][3] - matrix[2][1], matrix[3][3] - matrix[3][1]);
            _planes[FP_BOTTOM].set(matrix[0][3] + matrix[0][1], matrix[1][3] + matrix[1][1], matrix[2][3] + matrix[2][1], matrix[3][3] + matrix[3][1]);
            _planes[FP_NEAR].set(matrix[0][3] + matrix[0][2], matrix[1][3] + matrix[1][2], matrix[2][3] + matrix[2][2], matrix[3][3] + matrix[3][2]);
            _planes[FP_FAR].set(matrix[0][3] - matrix[0][2], matrix[1][3] - matrix[1][2], matrix[2][3] - matrix[2][2], matrix[3][3] - matrix[3][2]);

            /*_planes[FP_LEFT].set(matrix[3][0] + matrix[0][0], matrix[3][1] + matrix[0][1], matrix[3][2] + matrix[0][2], matrix[3][3] + matrix[0][3]);
            _planes[FP_RIGHT].set(matrix[3][0] - matrix[0][0], matrix[3][1] - matrix[0][1], matrix[3][2] - matrix[0][2], matrix[3][3] - matrix[0][3]);
            _planes[FP_TOP].set(matrix[3][0] - matrix[1][0], matrix[3][1] - matrix[1][1], matrix[3][2] - matrix[1][2], matrix[3][3] - matrix[1][3]);
            _planes[FP_BOTTOM].set(matrix[3][0] + matrix[1][0], matrix[3][1] + matrix[1][1], matrix[3][2] + matrix[1][2], matrix[3][3] + matrix[1][3]);
            _planes[FP_NEAR].set(matrix[3][0] + matrix[2][0], matrix[3][1] + matrix[2][1], matrix[3][2] + matrix[2][2], matrix[3][3] + matrix[2][3]);
            _planes[FP_FAR].set(matrix[3][0] - matrix[2][0], matrix[3][1] - matrix[2][1], matrix[3][2] - matrix[2][2], matrix[3][3] - matrix[2][3]);*/

            normalizePlanes();
        }

        const Plane& getPlane(FrustumPlane plane)
        {
            return _planes[plane];
        }

        glm::vec3* const getPoints()
        {
            if (!_isCalculatedPoints)
            {
                calculatePoints();

                _isCalculatedPoints = true;
            }

            return _points;
        }

        void applyTransform(glm::mat4 transform)
        {
            glm::vec3* points = getPoints();

            for (int i = 0; i < 8; ++i)
            {
                glm::vec4 v = transform * glm::vec4(points[i], 1.0f);
                _points[i] = glm::vec3(v.x, v.y, v.z);
            }
        }


        AABB* getAABB()
        {
            glm::vec3* frustumPoints = _points;

            glm::vec3 min(FLT_MAX, FLT_MAX, FLT_MAX);
            glm::vec3 max(-FLT_MAX, -FLT_MAX, -FLT_MAX);

            for (int i = 0; i < 8; ++i)
            {
                if (frustumPoints[i].x < min.x)
                    min.x = frustumPoints[i].x;
                if (frustumPoints[i].x > max.x)
                    max.x = frustumPoints[i].x;

                if (frustumPoints[i].y < min.y)
                    min.y = frustumPoints[i].y;
                if (frustumPoints[i].y > max.y)
                    max.y = frustumPoints[i].y;

                if (frustumPoints[i].z < min.z)
                    min.z = frustumPoints[i].z;
                if (frustumPoints[i].z > max.z)
                    max.z = frustumPoints[i].z;
            }

            _aabb.setSize(min, max);

            return &_aabb;
        }

};


#endif // FRUSTUM_H_INCLUDED
