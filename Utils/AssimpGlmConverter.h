#ifndef ASSIMPGLMCONVERTER_H_INCLUDED
#define ASSIMPGLMCONVERTER_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/vector3.h>


namespace AssimpGlmConverter
{
	glm::mat4 toMat4(const aiMatrix4x4& assimpMatrix);

	inline glm::vec3 toVec3(const aiVector3D& assimpVector)
	{
		return glm::vec3(assimpVector.x, assimpVector.y, assimpVector.z);
	}

	inline glm::vec4 toVec4(const aiVector3D& assimpVector)
	{
		return glm::vec4(assimpVector.x, assimpVector.y, assimpVector.z, 1.0f);
	}

	inline glm::quat toQuat(const aiQuaternion& assimpQuaternion)
	{
		return glm::quat(assimpQuaternion.w, assimpQuaternion.x, assimpQuaternion.y, assimpQuaternion.z);
	}
};


#endif // ASSIMPGLMCONVERTER_H_INCLUDED
