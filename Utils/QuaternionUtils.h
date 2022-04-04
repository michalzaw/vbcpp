#ifndef QUATERNIONUTILS_H_INCLUDED
#define QUATERNIONUTILS_H_INCLUDED


#include <glm/glm.hpp>


namespace QuaternionUtils
{
	// Returns a quaternion q such that q * startVector = destinationVector
	// Vector startVector and destinationVector should be normalized
	// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/
	// Stan Melax's Game Programming Gems 1 article: (http://www.cesarkallas.net/arquivos/livros/informatica/game/Game%20Programming%20Gems%201.pdf p. 211)
	glm::quat rotationBetweenVectors(const glm::vec3& startVector, const glm::vec3& destinationVector);
}


#endif // QUATERNIONUTILS_H_INCLUDED