#include "RaycastingUtils.h"

#include "Collision.h"


void calculateRay(double x, double y, CameraStatic* camera, glm::vec3& rayStart, glm::vec3& rayDir)
{
    glm::vec4 rayStartNDC((x / camera->getWindowWidth() - 0.5f) * 2.0f,
                          (y / camera->getWindowHeight() - 0.5f) * 2.0f,
                          -1.0f,
                          1.0f);

    glm::vec4 rayEndNDC((x / camera->getWindowWidth() - 0.5f) * 2.0f,
                        (y / camera->getWindowHeight() - 0.5f) * 2.0f,
                        0.0f,
                        1.0f);

    glm::mat4 viewProjectionInv = glm::inverse(camera->getProjectionMatrix() * camera->getViewMatrix());

    glm::vec4 rayStartWorldspace = viewProjectionInv * rayStartNDC;
    rayStartWorldspace /= rayStartWorldspace.w;

    glm::vec4 rayEndWorldspace = viewProjectionInv * rayEndNDC;
    rayEndWorldspace /= rayEndWorldspace.w;

    rayStart = glm::vec3(rayStartWorldspace);
    rayDir = glm::normalize(glm::vec3(rayEndWorldspace - rayStartWorldspace));
}
