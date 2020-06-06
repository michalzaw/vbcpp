#ifndef RAYCASTINGUTILS_H_INCLUDED
#define RAYCASTINGUTILS_H_INCLUDED


#include <GL/glew.h>

#include "../Graphics/CameraStatic.hpp"


// Calculate ray begin and direction in world space. x, y - mouse position in screen space
void calculateRay(double x, double y, CameraStatic* camera, glm::vec3& rayStart, glm::vec3& rayDir);


#endif // RAYCASTINGUTILS_H_INCLUDED
