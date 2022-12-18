#include "QuaternionUtils.h"

#include <glm/gtx/vector_angle.hpp>

#include "Math.h"


namespace QuaternionUtils
{
	const float EPSILON = 0.001f;
	const float EPSILON2 = 0.01f;


	glm::quat rotationBetweenVectors(const glm::vec3& startVector, const glm::vec3& destinationVector)
	{
		float cosTheta = glm::dot(startVector, destinationVector);

		if (cosTheta < -1.0f + EPSILON)
		{
			// special case when vectors in opposite directions:
			glm::vec3 rotationAxis = glm::cross(glm::vec3(0.0f, 0.0f, 1.0f), startVector);
			if (glm::length2(rotationAxis) < EPSILON2) // they were parallel
			{
				rotationAxis = glm::cross(glm::vec3(1.0f, 0.0f, 0.0f), startVector);
			}

			rotationAxis = normalize(rotationAxis);

			return glm::angleAxis(degToRad(180.0f), rotationAxis);
		}

		glm::vec3 rotationAxis = glm::cross(startVector, destinationVector);

		float s = sqrt((1 + cosTheta) * 2);
		float invs = 1 / s;

		return glm::quat(s * 0.5f,
						 rotationAxis.x * invs,
						 rotationAxis.y * invs,
						 rotationAxis.z * invs);
	}
}
