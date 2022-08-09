#include "AssimpGlmConverter.h"


namespace AssimpGlmConverter
{
	glm::mat4 toMat4(const aiMatrix4x4& assimpMatrix)
    {
        glm::mat4 result;
        result[0][0] = assimpMatrix.a1; result[1][0] = assimpMatrix.a2; result[2][0] = assimpMatrix.a3; result[3][0] = assimpMatrix.a4;
        result[0][1] = assimpMatrix.b1; result[1][1] = assimpMatrix.b2; result[2][1] = assimpMatrix.b3; result[3][1] = assimpMatrix.b4;
        result[0][2] = assimpMatrix.c1; result[1][2] = assimpMatrix.c2; result[2][2] = assimpMatrix.c3; result[3][2] = assimpMatrix.c4;
        result[0][3] = assimpMatrix.d1; result[1][3] = assimpMatrix.d2; result[2][3] = assimpMatrix.d3; result[3][3] = assimpMatrix.d4;

        return result;
    }
};