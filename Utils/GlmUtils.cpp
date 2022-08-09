#include "GlmUtils.h"


namespace GlmUtils
{
	glm::vec3 getTranslationFromMatrix(const glm::mat4& transformMatrix)
	{
		glm::vec3 scale;
		glm::quat orientation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transformMatrix, scale, orientation, translation, skew, perspective);

		return translation;
	}


	glm::quat getRotationFromMatrix(const glm::mat4& transformMatrix)
	{
		glm::vec3 scale;
		glm::quat orientation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transformMatrix, scale, orientation, translation, skew, perspective);

		// https://stackoverflow.com/questions/17918033/glm-decompose-mat4-into-translation-and-rotation
		// funkcja glm::decompose zwraca niepoprawny kwaternion
		// aby otrzymaæ prawid³ow¹ rotacjê nale¿y wykonaæ glm::conjugate
		// conjugate tworzy tak naprawdê rotacjê odwrotn¹
		// dlatego te¿ w ObjectPropertiesWindow dzia³a³ poni¿szy kod:
		// glm::vec3 rotationEulerAngles = glm::eulerAngles(getRotation(nodeData->transformation));
		// rotationEulerAngles = glm::vec3(-rotationEulerAngles.x, -rotationEulerAngles.y, -rotationEulerAngles.z);
		// jeœli jednak chcemy operowaæ na kwaternionie, to lepiej wykonaæ glm::conjugate(orientation);
		return glm::conjugate(orientation);
	}


	glm::vec3 getScaleFromMatrix(const glm::mat4& transformMatrix)
	{
		glm::vec3 scale;
		glm::quat orientation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transformMatrix, scale, orientation, translation, skew, perspective);

		return scale;
	}


	void decomposeMatrix(const glm::mat4& transformMatrix, glm::vec3& outTranslation, glm::quat& outOrientation, glm::vec3& outScale)
	{
		glm::vec3 skew;
		glm::vec4 perspective;

		glm::decompose(transformMatrix, outScale, outOrientation, outTranslation, skew, perspective);

		outOrientation = glm::conjugate(outOrientation);
	}
}
