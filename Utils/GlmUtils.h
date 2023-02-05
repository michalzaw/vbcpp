#ifndef GLMUTILS_H_INCLUDED
#define GLMUTILS_H_INCLUDED


#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>


namespace GlmUtils
{
	glm::vec3 getTranslationFromMatrix(const glm::mat4& transformMatrix);
	glm::quat getRotationFromMatrix(const glm::mat4& transformMatrix);
	glm::vec3 getScaleFromMatrix(const glm::mat4& transformMatrix);

	void decomposeMatrix(const glm::mat4& transformMatrix, glm::vec3& outTranslation, glm::quat& outOrientation, glm::vec3& outScale);
}


#endif // GLMUTILS_H_INCLUDED
