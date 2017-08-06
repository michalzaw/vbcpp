#ifndef FRUSTUM_H_INCLUDED
#define FRUSTUM_H_INCLUDED


#include <glm/glm.hpp>


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


class Frustum
{
    private:
        Plane _planes[6];

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

    public:
        Frustum()
        {

        }

        Frustum(glm::mat4 matrix)
        {
            set(matrix);
        }

        void set(glm::mat4 matrix)
        {
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

};


#endif // FRUSTUM_H_INCLUDED
