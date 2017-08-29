#include "Frustum.h"


template <typename destT, typename srcT>
destT &absolute_cast(srcT &v)
{
	return reinterpret_cast<destT&>(v);
}
template <typename destT, typename srcT>
const destT &absolute_cast(const srcT &v)
{
	return reinterpret_cast<const destT&>(v);
}

#define FLOAT_ALMOST_ZERO(F) ((absolute_cast<unsigned>(F) & 0x7f800000L) == 0)


glm::vec3 intersectPlanes(const Plane& p1, const Plane& p2, const Plane& p3)
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

glm::vec3 intersectPlanes2(const Plane& P1, const Plane& P2, const Plane& P3)
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
