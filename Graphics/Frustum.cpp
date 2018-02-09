#include "Frustum.h"

#include "CameraStatic.hpp"


Frustum::Frustum()
{
    _isCalculatedPoints = false;
}


Frustum::Frustum(glm::mat4 matrix)
{
    _isCalculatedPoints = false;

    set(matrix);
}

#ifdef INTERSECT_PLANES_FUNCTION_1
glm::vec3 Frustum::intersectPlanes(const Plane& p1, const Plane& p2, const Plane& p3)
{
    float w = p1.a * p2.b * p3.c + p2.a * p3.b * p1.c + p3.a * p1.b * p2.c -
              p3.a * p2.b * p1.c - p1.a * p3.b * p2.c - p2.a * p1.b * p3.c;

    float wx = (-p1.d) * p2.b * p3.c + (-p2.d) * p3.b * p1.c + (-p3.d) * p1.b * p2.c -
               (-p3.d) * p2.b * p1.c - (-p1.d) * p3.b * p2.c - (-p2.d) * p1.b * p3.c;

    float wy = p1.a * (-p2.d) * p3.c + p2.a * (-p3.d) * p1.c + p3.a * (-p1.d) * p2.c -
               p3.a * (-p2.d) * p1.c - p1.a * (-p3.d) * p2.c - p2.a * (-p1.d) * p3.c;

    float wz = p1.a * p2.b * (-p3.d) + p2.a * p3.b * (-p1.d) + p3.a * p1.b * (-p2.d) -
               p3.a * p2.b * (-p1.d) - p1.a * p3.b * (-p2.d) - p2.a * p1.b * (-p3.d);

    return glm::vec3(wx / w, wy / w, wz / w);
}
#endif // INTERSECT_PLANES_FUNCTION_1


// Faster
// http://asawicki.info/productions/artykuly/Zaawansowana_kamera_3D.php5
#ifdef INTERSECT_PLANES_FUNCTION_2
glm::vec3 Frustum::intersectPlanes(const Plane& P1, const Plane& P2, const Plane& P3)
{
    float fDet;
    float MN[9]  = { P1.a, P1.b, P1.c, P2.a, P2.b, P2.c, P3.a, P3.b, P3.c };
    float IMN[9] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
    float MD[3]  = { -P1.d, -P2.d , -P3.d };

    IMN[0] =   MN[4] * MN[8] - MN[5] * MN[7];
    IMN[3] = -(MN[3] * MN[8] - MN[5] * MN[6]);
    IMN[6] =   MN[3] * MN[7] - MN[4] * MN[6];

    fDet = MN[0] * IMN[0] + MN[1] * IMN[3] + MN[2] * IMN[6];

    //if(FLOAT_ALMOST_ZERO(fDet))
    //    return false;

    IMN[1] = -(MN[1] * MN[8] - MN[2] * MN[7]);
    IMN[4] =   MN[0] * MN[8] - MN[2] * MN[6];
    IMN[7] = -(MN[0] * MN[7] - MN[1] * MN[6]);
    IMN[2] =   MN[1] * MN[5] - MN[2] * MN[4];
    IMN[5] = -(MN[0] * MN[5] - MN[2] * MN[3]);
    IMN[8] =   MN[0] * MN[4] - MN[1] * MN[3];

    fDet = 1.0f / fDet;

    IMN[0] *= fDet;
    IMN[1] *= fDet;
    IMN[2] *= fDet;
    IMN[3] *= fDet;
    IMN[4] *= fDet;
    IMN[5] *= fDet;
    IMN[6] *= fDet;
    IMN[7] *= fDet;
    IMN[8] *= fDet;

    glm::vec3 out;

    out.x = IMN[0] * MD[0] + IMN[1] * MD[1] + IMN[2] * MD[2];
    out.y = IMN[3] * MD[0] + IMN[4] * MD[1] + IMN[5] * MD[2];
    out.z = IMN[6] * MD[0] + IMN[7] * MD[1] + IMN[8] * MD[2];

    return out;
   //return true;
}
#endif // INTERSECT_PLANES_FUNCTION_2


void Frustum::normalizePlanes()
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

void Frustum::calculatePoints()
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


void Frustum::set(glm::mat4 matrix)
{
    _isCalculatedPoints = false;

    _planes[FP_LEFT].set(matrix[0][3] + matrix[0][0], matrix[1][3] + matrix[1][0], matrix[2][3] + matrix[2][0], matrix[3][3] + matrix[3][0]);
    _planes[FP_RIGHT].set(matrix[0][3] - matrix[0][0], matrix[1][3] - matrix[1][0], matrix[2][3] - matrix[2][0], matrix[3][3] - matrix[3][0]);
    _planes[FP_TOP].set(matrix[0][3] - matrix[0][1], matrix[1][3] - matrix[1][1], matrix[2][3] - matrix[2][1], matrix[3][3] - matrix[3][1]);
    _planes[FP_BOTTOM].set(matrix[0][3] + matrix[0][1], matrix[1][3] + matrix[1][1], matrix[2][3] + matrix[2][1], matrix[3][3] + matrix[3][1]);
    _planes[FP_NEAR].set(matrix[0][3] + matrix[0][2], matrix[1][3] + matrix[1][2], matrix[2][3] + matrix[2][2], matrix[3][3] + matrix[3][2]);
    _planes[FP_FAR].set(matrix[0][3] - matrix[0][2], matrix[1][3] - matrix[1][2], matrix[2][3] - matrix[2][2], matrix[3][3] - matrix[3][2]);


    normalizePlanes();
}

void Frustum::setPoints(CameraStatic* camera)
{
    _isCalculatedPoints = true;

    float ratio = (float)camera->getWindowWidth() / (float)camera->getWindowHeight();
    float heightNear = 2 * tan(camera->getViewAngle() / 2.0f) * camera->getNearValue();
    float widthNear = heightNear * ratio;

    float heightFar =  2 * tan(camera->getViewAngle() / 2.0f) * camera->getFarValue();
    float widthFar = heightFar * ratio;

    glm::vec3 fc = camera->getPosition() + camera->getDirection() * camera->getFarValue();

    _points[FP_FAR_LEFT_TOP] = fc + (camera->getUpVector() * heightFar / 2.0f) - (camera->getRightVector() * widthFar / 2.0f);
    _points[FP_FAR_RIGHT_TOP] = fc + (camera->getUpVector() * heightFar / 2.0f) + (camera->getRightVector() * widthFar / 2.0f);
    _points[FP_FAR_LEFT_BOTTOM] = fc - (camera->getUpVector() * heightFar / 2.0f) - (camera->getRightVector() * widthFar / 2.0f);
    _points[FP_FAR_RIGHT_BOTTOM] = fc - (camera->getUpVector() * heightFar / 2.0f) + (camera->getRightVector() * widthFar / 2.0f);

    glm::vec3 nc = camera->getPosition() + camera->getDirection() * camera->getNearValue();

    _points[FP_NEAR_LEFT_TOP] = nc + (camera->getUpVector() * heightNear / 2.0f) - (camera->getRightVector() * widthNear / 2.0f);
    _points[FP_NEAR_RIGHT_TOP] = nc + (camera->getUpVector() * heightNear / 2.0f) + (camera->getRightVector() * widthNear / 2.0f);
    _points[FP_NEAR_LEFT_BOTTOM] = nc - (camera->getUpVector() * heightNear / 2.0f) - (camera->getRightVector() * widthNear / 2.0f);
    _points[FP_NEAR_RIGHT_BOTTOM] = nc - (camera->getUpVector() * heightNear / 2.0f) + (camera->getRightVector() * widthNear / 2.0f);
}

const Plane& Frustum::getPlane(FrustumPlane plane)
{
    return _planes[plane];
}


glm::vec3* const Frustum::getPoints()
{
    if (!_isCalculatedPoints)
    {
        calculatePoints();

        _isCalculatedPoints = true;
    }

    return _points;
}


void Frustum::applyTransform(const glm::mat4& transform)
{
    glm::vec3* points = getPoints();

    for (int i = 0; i < 8; ++i)
    {
        glm::vec4 v = transform * glm::vec4(points[i], 1.0f);
        _points[i] = glm::vec3(v.x, v.y, v.z);
    }
}


AABB* Frustum::getAABB()
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
