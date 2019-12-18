#include "CameraSettingsWindow.h"


/*
CameraSettingsWindow::CameraSettingsWindow(SceneManager* sceneManager, SceneObject*& selectedSceneObject, bool isOpen)
    : EditorWindow(sceneManager, selectedSceneObject, isOpen)
{

}


void CameraSettingsWindow::drawWindow()
{
    if (ImGui::Begin("Camera Settings", &_isOpen))
    {
        CameraStatic* camera = GraphicsManager::getInstance().getCurrentCamera();
        CameraFPS* cameraFPS = dynamic_cast<CameraFPS*>(camera);
        if (cameraFPS != nullptr)
        {
            float movementSpeed = cameraFPS->getMoveSpeed();
            float rotationSpeed = cameraFPS->getRotationSpeed();

            ImGui::DragFloat("Movement speed", &movementSpeed, 0.01f, 0.0f, 10.0f);

            // Rotation
            ImGui::DragFloat("Rotation speed", &rotationSpeed, 0.01f, 0.0f, 1.0f);

            cameraFPS->setMoveSpeed(movementSpeed);
            cameraFPS->setRotationSpeed(rotationSpeed);
        }
    }
    ImGui::End();
}
*/
void CameraSettingsWindow(CameraFPS* camera)
{
	if (camera)
	{
		float movementSpeed = camera->getMoveSpeed();
		float rotationSpeed = camera->getRotationSpeed();

		ImGui::DragFloat("Movement speed", &movementSpeed, 0.01f, 0.0f, 20.0f);

		// Rotation
		ImGui::DragFloat("Rotation speed", &rotationSpeed, 0.01f, 0.0f, 1.0f);

		camera->setMoveSpeed(movementSpeed);
		camera->setRotationSpeed(rotationSpeed);
	}
}